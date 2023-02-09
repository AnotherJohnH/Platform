#!/bin/sh
#
# Install template project files into a new project directory

#-----------------------------------------------------------
# install a file with some m4 macro substitution

installFile()
{
   from=$1
   to_dir=$2
   to_file=$3

   echo "Install $from to $to_dir/$to_file"

   m4 \
      -DAUTHOR="$author" \
      -DYEAR="$year" \
      -DUSERNAME="$username" \
      -DAPPLICATION="$application" \
      -DVERSION="$version" \
      -DDESCRIPTION="$description" \
      -DLINK="$link" \
      -DBINARY="$binary" \
      $from > $to_dir/$to_file
}

#-----------------------------------------------------------
# Default configuration values

def_project_dir=`cd ..; pwd`
def_year=`date +%Y`
def_author="John D. Haughton"
def_username="AnotherJohnH"
def_application=`echo $def_project_dir | sed 's/.*\///'`
def_version="0.0.0"
def_description="Empty application"
def_link="https://github.com/AnotherJohnH/"
def_binary=`echo $def_application | tr '[:upper:]' '[:lower:]'`
def_source=consoleApp.cpp

#-----------------------------------------------------------
# Configure

echo
read -p "Project dir [$def_project_dir] : " project_dir; project_dir=${target:-$def_project_dir}
read -p "Copyright owner [$def_author] : "  author;      author=${author:-$def_author}
read -p "Copyright year [$def_year] : "     year;        year=${year:-$def_year}
read -p "User name [$def_username] : "      username;    username=${username:-$def_username}
read -p "Application [$def_application] : " application; application=${application:-$def_application}
read -p "Version [$def_version] : "         version;     version=${version:-$def_version}
read -p "Description [$def_description] : " description; description=${description:-$def_description}
read -p "Link [$def_link] : "               link;        link=${link:-$def_link}
read -p "Binary [$def_binary] : "           binary;      binary=${binary:-$def_binary}
                                                         source=${source:-$def_source}
echo

#-----------------------------------------------------------
# install top-level files

mkdir -p ${project_dir}
for name  in `ls -A scripts/template`
do
   if [ ! -d $name  ]
   then
      installFile scripts/template/$name  $project_dir $name
   fi
done

#-----------------------------------------------------------
# Install a github action

mkdir -p ${project_dir}/.github/workflows
installFile scripts/template/.github/workflows/build_on_push.yml \
            ${project_dir}/.github/workflows \
            build_on_push.yml

#-----------------------------------------------------------
# install a main source file

mkdir -p ${project_dir}/Source
installFile scripts/template/Source/${source} ${project_dir}/Source ${binary}.cpp
echo
