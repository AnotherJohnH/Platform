#!/usr/bin/env python3
#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

import datetime
import pathlib
import re
import subprocess

#-------------------------------------------------------------------------------

def prompt(label, default):
    """ Prompt for a value with a shell-like default """

    value = input(f"{label} [{default}] : ").strip()
    return value or default

#-------------------------------------------------------------------------------

def git_config(name, default):
    """ Return a git config value, or default if it is not configured. """

    try:
        result = subprocess.run(
            ["git", "config", "--get", name],
            capture_output=True,
            text=True,
        )
    except OSError:
        return default

    value = result.stdout.strip()
    return value or default

#-------------------------------------------------------------------------------

def chooseTemplate(script_dir):

    template_options = {
        path.name.removeprefix("template_"): path
        for path in sorted(script_dir.glob("template_*"))
        if path.is_dir()
    }

    if not template_options:
        raise FileNotFoundError(f"No template directories found in '{script_dir}'")

    print("Template options:")
    for index, name in enumerate(template_options, start=1):
        print(f"  {index}. {name}")

    while True:
        selection = input("Template [1] : ").strip() or "1"

        if selection.isdigit():
            index = int(selection)
            if 1 <= index <= len(template_options):
                name = list(template_options)[index - 1]
                return template_options[name]

        selection = selection.upper()
        if selection in template_options:
            return template_options[selection]

        print("Please select a template by number or name.")

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

    print(f"Install '{destination_name}' to '{destination_dir}'")

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
    project_dir  = pathlib.Path(prompt("Project dir", project_dir)).expanduser()

    template_dir = chooseTemplate(script_dir)

    def_author   = git_config("user.name", "John D. Haughton")
    def_username = git_config("user.email", "SloeComputers").split("@", 1)[0]
    def_year     = str(datetime.datetime.now().year)
    def_link     = f"https://github.com/{def_username}"

    details = {}
    details["AUTHOR"]      = prompt("Copyright owner", def_author)
    details["YEAR"]        = prompt("Copyright year", def_year)
    details["USERNAME"]    = prompt("User name", def_username)
    details["APPLICATION"] = prompt("Application", project_dir.name)
    details["VERSION"]     = prompt("Version", "0.0")
    details["DESCRIPTION"] = prompt("Description", "Example application")
    details["LINK"]        = prompt("Link", def_link)
    details["BINARY"]      = prompt("Binary", project_dir.name.lower())

    print()

    project_dir.mkdir(parents=True, exist_ok=True)

    install_dir(template_dir, project_dir, details)

    print()

#-------------------------------------------------------------------------------

if __name__ == "__main__":
    main()
