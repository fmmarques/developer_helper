#!.venv/bin/python

import subprocess
import click
import os
from abc import ABC, abstractmethod

class Project:
    def __init__(self):
        self.generate = None

    def __str__(self):
        return f'project generate: {self.generate}'

class Build:
    def __init__(self):
        self.all = None
        self.clean = None
        self.rebuild = None

    def __str__(self):
        return  f'build all: {self.all}'\
                f'build clean: {self.clean}'\
                f'build rebuild: {self.rebuild}'
class Test:
    def __init__(self):
        self.all = None

    def __str__(self):
        return  f'test all: {self.all}'

class Generator:
    def __init__(self, name, source_directory, build_directory):
        self.name = name
        self.build_directory = build_directory
        self.source_directory = source_directory
        self.project = Project()
        self.build = Build()
        self.test = Test()

    def __str__(self):
        return  f'name : {self.name}'\
                f'source_dir: {self.source_directory}'\
                f'build_directory: {self.build_directory}'\
                f'project: {self.project}'\
                f'build: {self.build}'\
                f'test: {self.test}'



class Builder:
    def construct_project(self):
        pass

    def construct_build(self):
        pass

    def construct_test(self):
        pass

    def get_generator(self):
        pass


class MakeGeneratorBuilder(Builder):
    def __init__(self,  source_directory='./', build_directory="./build/make"):
        self.generator = Generator("Unix Makefiles", source_directory, build_directory)

    def construct_project(self):
        self.generator.project.generate = f'cmake -G"{self.generator.name}" -B"{self.generator.build_directory}" -S"{self.generator.source_directory}"'

    def construct_build(self):
        self.generator.build.all = f"make -C '{self.generator.build_directory}/' all"
        self.generator.build.clean = f"make -C '{self.generator.build_directory}/' clean"
        self.generator.build.rebuild = self.generator.build.clean + " ; " + self.generator.build.all

    def construct_test(self):
        self.generator.test.all = f"make -C '{self.generator.build_directory}' test"

    def get_generator(self):
        return self.generator

class VisualStudioGeneratorBuilder(Builder):
    def __init__(self, source_directory='./', build_directory="./build/vs", version=16, year=2022 ):
        self.generator = Generator("Visual Studio {version} {year}", build_directory=build_directory, source_directory=source_directory)
        self.generator.version = version
        self.generator.year = year

    def construct_project(self):
        self.generator.project.generate = f'cmake -G"{self.generator.name}" -B"{self.generator.build_directory}" -S"{self.generator.source_directory}"'

    def construct_build(self):
        raise RuntimeError("Not implemented")
        self.generator.build.all = f"msbuild "
        self.generator.build.clean = f"msbuild"
        self.generator.build.rebuild = f"msbuild"

    def construct_test(self):
        raise RuntimeError("Not implemented")
        self.generator.test.all = f"make -C '{self.generator.build_directory}' test"

    def get_generator(self):
        return self.generator

# Creator interface
class GeneratorBuilder(ABC):
    @abstractmethod
    def create_generator(self):
        pass

    def get_generator(self, *kargs, **kwargs):
        generator = self.create_generator(*kargs, **kwargs)
        generator.construct_project()
        generator.construct_build()
        generator.construct_test()
        return generator.get_generator()

# Concrete Creators
class DefaultGeneratorCreator(GeneratorBuilder):
    def create_generator(self, source_directory="./", build_directory="./build/"):
        return MakeGeneratorBuilder(source_directory, build_directory)

# Concrete Creators
class MakeGeneratorCreator(GeneratorBuilder):
    def create_generator(self, source_directory="./", build_directory="./build/"):
        return MakeGeneratorBuilder(source_directory, build_directory)

class VisualStudioGeneratorCreator(GeneratorBuilder):
    def create_generator(self, source_directory="./", build_directory="./build/", version=16, year=2022):
        return VisualStudioGeneratorBuilder(source_directory, build_directory, version, year)

def make_generator_from_creator_class(generator_creator_cls, *args, **kwargs):
    try:
        # Get the class object by name
        cls = globals()[generator_creator_cls]

        # Instantiate the class with any additional arguments
        instance = cls()
        # Return the generator
        return instance.get_generator(*args, **kwargs)

    except KeyError:
        print(f"Class '{generator_creator_cls}' not found.")
    except Exception as e:
        print(f"Error instantiating class '{generator_creator_cls}': {e}")

def do_command(cmd, dry_run=False):
    if (dry_run):
        click.echo(cmd)
        return
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    for line in iter(process.stdout.readline, b''):
        print(line.decode('utf-8').strip())
    for line in iter(process.stderr.readline, b''):
        print(line.decode('utf-8').strip())
    process.communicate()


@click.group(help="Use \"./make.py <COMMAND>\" for more information")
def cli():
    pass

@cli.group(help="Project related commands. Use \"./make.py project <SUBCOMMAND>\" to learn about the supported subcommand.")
def project():
    pass

@project.command(help="Creates the project onto \"./build\" directory.")
@click.option('--project_type', default="make", type=str, help="")
@click.option('--source_directory', default=".", type=str, help="The directory where the project source is (containing CMakeLists.txt)")
@click.option('--build_directory', default="./build", type=str, help="The directory where the project will be built")
@click.option('--dry_run', default=False, type=bool, help="Echo the cmake command")
def generate(project_type, source_directory, build_directory, dry_run):
    cls = "DefaultGeneratorCreator"
    if (project_type == "make"):
        cls = "MakeGeneratorCreator"
    elif (project_type == "vs" or project_type == "vsts" or project_type == "visualstudio"):
        cls = "VisualStudioGeneratorCreator"
    generator = make_generator_from_creator_class(cls, build_directory=build_directory, source_directory=source_directory)
    do_command(generator.project.generate, dry_run=dry_run)

def make_generator_from_filesystem(source_directory, build_directory):
    def make_makegenerator_from_filesystem(source_directory, build_directory):
        def does_makefile_exists(build_directory):
            return os.path.exists(f"{build_directory}/Makefile")

        if (not does_makefile_exists(build_directory)):
            raise RuntimeError(f"Couldn't find a Makefile on {build_directory}. Try running \"{__file__} project make" "source_directory {source_directory} --build_directory {build_directory}\"")
        generator = make_generator_from_creator_class("MakeGeneratorCreator", build_directory=build_directory, source_directory=source_directory)
        return generator

    def make_vsgenerator_from_filesystem(source_directory, build_directory):
        raise RuntimeError("Not implemented")

    generator = None
    try:
        generator = make_makegenerator_from_filesystem(source_directory, build_directory)
    except RuntimeError as re:
        print(e)
        pass # Expected to exception if not found.

    try:
        generator = make_vsgenerator_from_filesystem(source_directory, build_directory)
    except RuntimeError as re:
        pass # Expected to exception if not found.

    if (generator is None):
        raise RuntimeError(f"Couldn't find a generated project on the specified build_directory ({build_directory}).")
    return generator

def ensure_project_exists():
    visualstudio_exists = False;
    if (os.path.exists("./build/current_vsst")):
        with open("./build/current_vsst", 'r') as file:
            vsts_dir = file.read()
        visualstudio_exists = os.path.exists(vsts_dir)
    return makefile_exists or visualstudio_exists

@cli.group(help="Build related commands. Use \"./make.py build\" to learn about the supported subcommands.")
def build():
    pass

@build.command()
@click.option('--source_directory', default=".", type=str, help="The directory where the project source is (containing CMakeLists.txt)")
@click.option('--build_directory', default="./build", type=str, help="The directory where the project will be built")
@click.option('--dry_run', default=False, type=bool, help="Echo the cmake command")
def all(source_directory, build_directory, dry_run):
    generator = make_generator_from_filesystem(source_directory, build_directory)
    do_command(generator.build.all, dry_run=dry_run)

@build.command()
@click.option('--source_directory', default=".", type=str, help="The directory where the project source is (containing CMakeLists.txt)")
@click.option('--build_directory', default="./build", type=str, help="The directory where the project will be built")
@click.option('--dry_run', default=False, type=bool, help="Echo the cmake command")
def clean(source_directory, build_directory, dry_run):
    generator = make_generator_from_filesystem(source_directory, build_directory)
    do_command(generator.build.clean, dry_run=dry_run)

@build.command()
@click.option('--source_directory', default=".", type=str, help="The directory where the project source is (containing CMakeLists.txt)")
@click.option('--build_directory', default="./build", type=str, help="The directory where the project will be built")
@click.option('--dry_run', default=False, type=bool, help="Echo the cmake command")
def rebuild(source_directory, build_directory, dry_run):
    generator = make_generator_from_filesystem(source_directory, build_directory)
    do_command(generator.build.clean, dry_run=dry_run)
    do_command(generator.build.all, dry_run=dry_run)

@cli.group(help="test related commands. Use \"./make.py test <SUBCOMMAND>\" to learn about the supported subcommands.")
def test():
    pass

@test.command()
@click.option('--source_directory', default=".", type=str, help="The directory where the project source is (containing CMakeLists.txt)")
@click.option('--build_directory', default="./build", type=str, help="The directory where the project will be built")
@click.option('--dry_run', default=False, type=bool, help="Echo the cmake command")
def all(source_directory, build_directory, dry_run):
    generator = make_generator_from_filesystem(source_directory, build_directory)
    do_command(generator.test.all, dry_run=dry_run)

if __name__ == '__main__':
    cli()
