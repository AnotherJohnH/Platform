#!/usr/bin/env python3
#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import datetime
import pathlib
import re

#-------------------------------------------------------------------------------

def prompt(label, default):
    """ Prompt for a value with a shell-like default """

    value = input(f"{label} [{default}] : ").strip()
    return value or default

#-------------------------------------------------------------------------------

def instantiate_template(text, values):
    """ Apply the small subset of m4 used by the template files """

    def stash(match):
        quoted.append(match.group(1))
        return f"@@M4_QUOTED_{len(quoted) - 1}@@"

    quoted = []
    text = re.sub(r"`([^']*)'", stash, text)

    for name in values:
        text = re.sub(rf"\b{name}\b", values[name], text)

    for index, literal in enumerate(quoted):
        text = text.replace(f"@@M4_QUOTED_{index}@@", literal)

    return text

#-------------------------------------------------------------------------------

def install_file(source, destination_dir, destination_name, values):
    """ Instantiate and install a template file. """

    print(f"Install '{source.name}' to '{destination_dir}'")

    destination = destination_dir / destination_name
    destination_dir.mkdir(parents=True, exist_ok=True)

    text = source.read_text()
    destination.write_text(instantiate_template(text, values))

#-------------------------------------------------------------------------------

def install_dir(name, destination_dir, values):
    """ Instantiate and install a directoy of files. """

    for file in name.iterdir():
        if file.is_dir():
           install_dir(file, destination_dir / file.name, values)
        else:
           filename = file.name.replace('BINARY', values['BINARY'])
           install_file(file, destination_dir, filename, values)

#-------------------------------------------------------------------------------

def main():

    print()

    project_dir  = pathlib.Path.cwd().parent.resolve()
    script_dir   = pathlib.Path(__file__).resolve().parent
    template_dir = script_dir / "template"
    projedt_dir  = prompt("Project dir", project_dir)

    def_author   = "John D. Haughton"
    def_username = "SloeComputers"
    def_year     = str(datetime.datetime.now().year)
    def_link     = f"https://github.com/{def_username}"

    details = {}
    details["AUTHOR"]      = prompt("Copyright owner", def_author)
    details["YEAR"]        = prompt("Copyright year", def_year)
    details["USERNAME"]    = prompt("User name", def_username)
    details["APPLICATION"] = prompt("Application", project_dir.name)
    details["VERSION"]     = prompt("Version", "0.0")
    details["DESCRIPTION"] = prompt("Description", "Initial program")
    details["LINK"]        = prompt("Link", def_link)
    details["BINARY"]      = prompt("Binary", project_dir.name.lower())

    print()

    project_dir = pathlib.Path(project_dir).expanduser()
    project_dir.mkdir(parents=True, exist_ok=True)

    install_dir(template_dir, project_dir, details)

    print()

#-------------------------------------------------------------------------------

if __name__ == "__main__":
    main()
