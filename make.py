#!python

import subprocess
import click

def do_command(cmd):
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    for line in iter(process.stdout.readline, b''):
        print(line.decode('utf-8').strip())
    process.communicate()


@click.group()
def cmake():
    pass

@click.command()
def run():
    do_command("cmake -B './build/'")

@click.command()
def refresh():
    do_command("cmake --fresh -B './build/'")

cmake.add_command(run)
cmake.add_command(refresh)

@click.group()
def make():
    pass

@click.command()
def all():
    do_command("make -C './build/'")

@click.command()
def clean():
    do_command("make -C './build/' clean")

@click.command()
def rebuild():
    do_command("make -C './build/' clean")
    do_command("make -C './build/' all")

make.add_command(all)
make.add_command(clean)
make.add_command(rebuild)

@click.group()
def test():
    pass

@click.command()
def runAll():
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
