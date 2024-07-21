#!.venv/bin/python

import subprocess
import click
import os
from abc import ABC, abstractmethod

class Generator:
    def __init__(self, name,  build_dir, source_dir):
        self.name = name
        self.build_dir = build_dir
        self.source_dir = source_dir
        self.generate_command = None
        self.build_command = None

    def __str__(self):
        return f'build_dir: {self.build_dir}' \
               f'source_dir: {self.source_dir}'

class Builder:
    def format_generate_command(self):
        pass

    def format_build_command(self):
        pass

    def get_generator(self):
        pass


class MakeGeneratorBuilder(Builder):
    def __init__(self, build_dir="./build/make", source_dir='./'):
        self.generator = Generator("Unix Makefiles", build_dir, source_dir)

    def format_generator_command(self):
        self.generator.generate_command = f'cmake -G"{self.generator.name}" -B"{self.generator.build_dir}" -S"{self.generator.source_dir}"'

    def format_build_command(self):
        self.generator.build_command = f"make -C '{self.generator_build_dir}/' all"

    def get_generator(self):
        return self.generator

class VisualStudioGeneratorBuilder(Builder):
    def __init__(self, version=16, year=2022, build_dir="./build/vs", source_dir='./'):
        self.generator = Generator("Visual Studio {version} {year}", build_dir, source_dir)
        self.generator.version = version
        self.generator.year = year

    def format_generator_command(self):
        self.generator.generate_command = f'cmake -G"{self.generator.name}" -B"{self.generator.build_dir}" -S"{self.generator.source_dir}"'

    def format_build_command(self):
        self.generator.build_command = 'Part 2'

    def get_generator(self):
        return self.generator

# Creator interface
class GeneratorBuilder(ABC):
    @abstractmethod
    def create_generator(self):
        pass

    def get_generator(self):
        generator = self.create_generator()
        generator.format_generator_command()
        generator.format_build_command()
        return generator.get_generator()

# Concrete Creators
class MakeGeneratorCreator(GeneratorBuilder):
    def create_generator(self, build_directory="./build/"):
        return MakeGeneratorBuilder()

class VisualStudioGeneratorCreator(GeneratorBuilder):
    def create_generator(self):
        return VisualStudioGeneratorBuilder()

def make_generator_from(generator_creator_cls, *args, **kwargs):
    try:
        # Get the class object by name
        cls = globals()[generator_creator_cls]

        # Instantiate the class with any additional arguments
        instance = cls(*args, **kwargs)

        # Return the generator
        return instance.get_generator()

    except KeyError:
        print(f"Class '{generator_creator_cls}' not found.")
    except Exception as e:
        print(f"Error instantiating class '{generator_creator_cls}': {e}")

def do_command(cmd, debug=False):
    if (debug):
        click.echo(cmd)
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    for line in iter(process.stdout.readline, b''):
        print(line.decode('utf-8').strip())
    process.communicate()


@click.group(help="Project related commands. Use \"./make.py project <SUBCOMMAND>\" to learn about the supported subcommand.")
def project():
    pass

@click.command(help="Creates default project onto \"./build\" directory.")
def make():
    generator_creator = make_generator_from(generator_cls="MakeGeneratorCreator", )
    do_command(generator.generate_command, debug=True)

@click.command(help="Creates Visual Studio project onto \"./build\" directory.")
@click.option('--version', default=17, type=int, help="The desired version of Visual Studio")
@click.option('--year', default=2022, type=int, help="The year of Visual Studio version")
def visual_studio(version, year):
    do_command(f"cmake -B './build/visual-studio-{version}-{year}' -G'Visual Studio {version} {year}' .")
    do_command(f"echo 'visual-studio-{version}-{year}' > ./build/current-vsst")


@click.command(help="Refresh default project on \"./build\" directory.")
def refresh_make():
    do_command("cmake --fresh -B './build/make' -G'Unix Makefiles' .")

project.add_command(make)
project.add_command(refresh_make)
project.add_command(visual_studio)

def ensure_project_exists():
    makefile_exists = os.path.exists("./build/make/Makefile")

    visualstudio_exists = False;
    if (os.path.exists("./build/current_vsst")):
        with open("./build/current_vsst", 'r') as file:
            vsts_dir = file.read()
        visualstudio_exists = os.path.exists(vsts_dir)
    return makefile_exists or visualstudio_exists

@click.group(help="Build related commands. Use \"./make.py build\" to learn about the supported subcommands.")
def build():
    pass

@click.command()
def all():
    ensure_project_exists()
    do_command("make -C './build/'")

@click.command()
def clean():
    ensure_makefile_exists()
    do_command("make -C './build/' clean")

@click.command()
def rebuild():
    ensure_makefile_exists()
    do_command("make -C './build/' clean")
    do_command("make -C './build/' all")

build.add_command(all)
build.add_command(clean)
build.add_command(rebuild)

@click.group(help="test related commands. Use \"./make.py test <SUBCOMMAND>\" to learn about the supported subcommands.")
def test():
    pass

@click.command()
def runAll():
    result = subprocess.run(['make', '-C', './build/', 'clean'], stdout=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))


@click.group(help="Use \"./make.py <COMMAND>\" for more information")
def verbs():
    pass

verbs.add_command(project)
verbs.add_command(build)
verbs.add_command(test)

if __name__ == '__main__':
    verbs()
