require 'rbuild-bundle'
require 'rbuild-cfamily'

objects = build_objects(:sources => Dir['Source/*.cpp'] +
                                    Dir['Source/*.m'] +
                                    Dir['Source/*.c'])

build_bundle(:bundle_name => 'Astro.app',
             :resources_directory => 'Resources')

build_executable(:executable => 'Astro.app/Contents/MacOS/Astro',
                 :objects => objects,
                 :frameworks => ['Cocoa', 'OpenGL', 'GLUT'])

bmps2rgba_objects = build_objects(:sources => Dir['Source/bmps2rgba/*.c'])

build_executable(:executable => 'bmps2rgba',
                 :objects => bmps2rgba_objects)