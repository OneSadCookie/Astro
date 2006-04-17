# You need scons version 0.96.91 or later for this SConstruct
# download from http://scons.org/

##############################################################################
# Your application's configuration goes here:

# The name of your application's executable and bundle.
APPLICATION_NAME = 'Astro'

# All your source is a child of this directory.  Can be .
SOURCE_DIRECTORY = 'Source'

# Where to put the intermediate build products.  Will be created if
# necessary.
BUILD_DIRECTORY = 'Build'

# Frameworks to link to
FRAMEWORKS = ['Cocoa', 'OpenGL', 'GLUT']

# Source files to exclude (including SOURCE_DIRECTORY in their path)
EXCLUDE_SOURCE = ['Source/bmps2rgba/bmps2rgba.c']

# Directories that should be merged to to create the Resources directory
# in the application bundle
RESOURCES_DIRECTORIES = ['Resources']

# Path to your Info.plist file
INFO_PLIST_FILE = 'Info.plist'

# Command to process your Info.plist file, or None for a straight copy
INFO_PLIST_FILTER = '''perl -pe "s|SVNVERSION|`svn info | \
    grep 'Last Changed Rev' | perl -pe 's|Last Changed Rev: (\d+)|\\1|'`|"'''

#############################################################################
# You shouldn't need to edit the rest of this file

import os, re

def each_file(path):
    for root, dirs, files in os.walk(path):
        if '.svn' in dirs:
            dirs.remove('.svn')
        if 'CVS' in dirs:
            dirs.remove('CVS')
        
        for file in files:
            yield root + '/' + file

def bundle_path():
    return APPLICATION_NAME + '.app'

def contents_path():
    return bundle_path() + '/Contents'

def executable_path():
    return contents_path() + '/MacOS/' + APPLICATION_NAME

def resources_path():
    return contents_path() + '/Resources'

def sdk_to_path(sdk):
    return '/Developer/SDKs/MacOSX' + sdk + '.sdk'

def make_base_env():
    base_env = Environment(ENV = {
        'PATH': os.environ['PATH'],
        'HOME': os.environ['HOME']
    })
    base_env.Replace(CCFLAGS = [
        '-O2',
        '-ffast-math',
        '-mdynamic-no-pic',
        '-Wall',
        '-Wextra',
        '-Wno-unused-parameter',
        '-Wnewline-eof',
        '-Werror'
    ])
    for framework in FRAMEWORKS:
        base_env.Append(LINKFLAGS = [
            '-framework', framework
        ])
    return base_env

def make_arch_env(env, options):
    arch_env = env.Copy()
    arch_env['ARCH'] = options['arch']
    arch_env.Append(CCFLAGS = [
        '-arch', options['arch'],
        '-isysroot', sdk_to_path(options['sdk'])
    ])
    arch_env.Append(LINKFLAGS = [
        '-arch', options['arch'],
        '-Wl,-syslibroot,' + sdk_to_path(options['sdk'])
    ])
    arch_env.Append(ENV = {
        'MACOSX_DEPLOYMENT_TARGET': options['deployment_target']
    })
    return arch_env

def find_source_files():
    SOURCE_FILE_REGEX = re.compile('^.*\.(c|m|mm|cc|cpp|cxx)$')
    
    source_files = []
    for path in each_file(SOURCE_DIRECTORY):
        if SOURCE_FILE_REGEX.match(path):
            source_files += [path]
               
    return source_files

def source_to_object(source, arch):
    return re.sub(
        '^' + re.escape(SOURCE_DIRECTORY),
        BUILD_DIRECTORY + '/' + arch + '/Objects',
        source) + '.o'

def build_object(env, source):
    return env.Object(
        source_to_object(source, env['ARCH']),
        source)

def copy_resources(env):
    for directory in RESOURCES_DIRECTORIES:
        for path in each_file(directory):
            env.Install(
                resources_path() + re.sub(
                    '^' + re.escape(directory) + '(.*)' +
                          re.escape(os.path.basename(path)) + '$',
                    '\\1',
                    path),
                path)

base_env = make_base_env()
ppc_env = make_arch_env(base_env, {
    'arch': 'ppc',
    'sdk': '10.3.9',
    'deployment_target': '10.3'
})
i386_env = make_arch_env(base_env, {
    'arch': 'i386',
    'sdk': '10.4u',
    'deployment_target': '10.4'
})

source_files = filter(lambda f: f not in EXCLUDE_SOURCE, find_source_files())
ppc_objects = map(lambda f: build_object(ppc_env, f), source_files)
i386_objects = map(lambda f: build_object(i386_env, f), source_files)

ppc_exe = ppc_env.Program(
    BUILD_DIRECTORY + '/ppc/' + APPLICATION_NAME,
    ppc_objects)
i386_exe = i386_env.Program(
    BUILD_DIRECTORY + '/i386/' + APPLICATION_NAME,
    i386_objects)
base_env.Command(
    executable_path(),
    [ppc_exe, i386_exe],
    'lipo -create -output $TARGET $SOURCES')

copy_resources(base_env)

base_env.Command(
    contents_path() + '/PkgInfo',
    [],
    'echo "APPL????" > $TARGET')

if INFO_PLIST_FILTER == None:
    base_env.Install(
        contents_path(),
        INFO_PLIST_FILE)
else:
    base_env.AlwaysBuild(base_env.Command(
        contents_path() + '/Info.plist',
        INFO_PLIST_FILE,
        'cat $SOURCE | ' + INFO_PLIST_FILTER + ' > $TARGET'))
