#!python

import subprocess
import click

@click.group()
def cmake():
    pass

@click.command()
def run():
    result = subprocess.run(['cmake', '-B', './build/'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))

@click.command()
def refresh():
    result = subprocess.run(['cmake', '--fresh', '-B', './build/'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))

cmake.add_command(run)
cmake.add_command(refresh)

@click.group()
def make():
    pass

@click.command()
def all():
    result = subprocess.run(['make', '-C', './build/'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))

@click.command()
def clean():
    result = subprocess.run(['make', '-C', './build/', 'clean'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))

@click.command()
def rebuild():
    result = subprocess.run(['make', '-C', './build/', 'clean'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))

make.add_command(all)
make.add_command(clean)
make.add_command(rebuild)

@click.group()
def test():
    pass

@click.command()
def runAll()
    result = subprocess.run(['make', '-C', './build/', 'clean'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))


@click.group()
def verbs():
    pass

verbs.add_command(cmake)
verbs.add_command(make)
verbs.add_command(test)

if __name__ == '__main__':
    verbs()
