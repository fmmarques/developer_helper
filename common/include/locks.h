/**
* A common base for locks.
* date: 10/04/2018
* author: Frederico Marques
*/
#pragma once
#if !defined(LOCKS_H)

#include <definitions.hpp>
#include <type_traits>
#if !defined _M_CEE
#	include <thread>
#else
#if defined(Yield)
#	undef Yield
#endif

namespace std
{
	class this_thread
	{
	public:
		static void yield()
		{
			System::Threading::Thread::Yield();
		}
	};
}
#endif


namespace collab {
	namespace locks {

		/**
		* Allows for .NET
		*/
		namespace interface1 {


			class rw_writer_lock_strategy
			{
			public:
				static void lock(System::Threading::ReaderWriterLock^ _lock)
				{
					bool repeat = false;
					auto timeout_in_milliseconds = 50;

					auto lock = _lock; // forces another reference.
					do
					{
						repeat = false;
						try
						{
							lock->AcquireWriterLock(timeout_in_milliseconds);
						}
						catch (System::ApplicationException ^e)
						{
							std::this_thread::yield();
							repeat = true;
						}
					} while (repeat);
				}

				static void unlock(System::Threading::ReaderWriterLock^ _lock)
				{
					bool repeat = false;
					auto timeout_in_milliseconds = 50;

					auto lock = _lock; // forces another reference.
					do
					{
						repeat = false;
						try
						{
							lock->ReleaseWriterLock();
						}
						catch (System::ApplicationException ^e)
						{
						}
					} while (repeat);
				}

			};

			class rw_reader_lock_strategy
			{
			public:
				static void lock(System::Threading::ReaderWriterLock^ _lock)
				{
					bool repeat = false;
					auto timeout_in_milliseconds = 50;

					auto lock = _lock; // forces another reference.
					do
					{
						repeat = false;
						try
						{
							lock->AcquireReaderLock(timeout_in_milliseconds);
						}
						catch (System::ApplicationException ^e)
						{
							repeat = true;
						}
					} while (repeat);
				}

				static void unlock(System::Threading::ReaderWriterLock^ _lock)
				{
					bool repeat = false;
					auto timeout_in_milliseconds = 50;

					auto lock = _lock; // forces another reference.
					do
					{
						repeat = false;
						try
						{
							lock->ReleaseReaderLock();
						}
						catch (System::ApplicationException ^e)
						{
						}
					} while (repeat);
				}

			};

			class rwslim_reader_lock_strategy
			{
			public:
				static void lock(System::Threading::ReaderWriterLockSlim^ _lock)
				{
					auto lock = _lock; // forces another reference.
					lock->EnterUpgradeableReadLock();
				}

				static void unlock(System::Threading::ReaderWriterLockSlim^ _lock)
				{
					auto lock = _lock;
					lock->ExitUpgradeableReadLock();
				}

			};

			class rwslim_writer_lock_strategy
			{
			public:
				static void lock(System::Threading::ReaderWriterLockSlim^ _lock)
				{
					auto lock = _lock; // forces another reference.
					lock->EnterWriteLock();
				}

				static void unlock(System::Threading::ReaderWriterLockSlim^ _lock)
				{
					auto lock = _lock; // forces another reference.
					lock->ExitWriteLock();
				}
			};

			template < class lock_t, class strategy_t > ref class scoped_lock
			{
			private:
				lock_t ^ _lock;
			public:
				using strategy = strategy_t;
				// available methods
				scoped_lock(lock_t^ l) :
					_lock(l)
				{
					strategy::lock(_lock);
				}

				~scoped_lock()
				{
					strategy::unlock(_lock);
				}
			};

			template < class strategy_t > ref class scoped_lock< System::Threading::ReaderWriterLock, strategy_t >
			{
			private:
				System::Threading::ReaderWriterLock^ _lock;
			public:
				using strategy = strategy_t;

				scoped_lock(System::Threading::ReaderWriterLock^ lock) :
					_lock(lock)
				{
					strategy::lock(_lock);
				}

				scoped_lock() :
					_lock(gcnew System::Threading::ReaderWriterLock())
				{
					strategy::lock(_lock);
				}

				~scoped_lock()
				{
					strategy::unlock(_lock);
				}
			};

			template < class strategy_t > ref class scoped_lock< System::Threading::ReaderWriterLockSlim, strategy_t >
			{
			private:
				System::Threading::ReaderWriterLockSlim^ _lock;
			public:
				using strategy = strategy_t;

				scoped_lock(System::Threading::ReaderWriterLockSlim^ lock) :
					_lock(lock)
				{
					strategy::lock(_lock);
				}

				scoped_lock() :
					_lock(gcnew System::Threading::ReaderWriterLock())
				{
					strategy::lock(_lock);
				}

				~scoped_lock()
				{
					strategy::unlock(_lock);
				}
			};

		}

		template < class lock_t, class strategy_t > using scoped_lock = ref class interface1::scoped_lock< lock_t, strategy_t>;
		using scoped_writer_lock = interface1::scoped_lock< System::Threading::ReaderWriterLock, interface1::rw_writer_lock_strategy >;
		using scoped_reader_lock = interface1::scoped_lock< System::Threading::ReaderWriterLock, interface1::rw_reader_lock_strategy >;
		using scoped_slimwriter_lock = scoped_lock< System::Threading::ReaderWriterLockSlim, interface1::rwslim_writer_lock_strategy >;
		using scoped_slimread_lock = scoped_lock< System::Threading::ReaderWriterLockSlim, interface1::rwslim_reader_lock_strategy >;
	}
}

#endif