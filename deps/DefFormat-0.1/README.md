# DefFormat
Simple library for parsing .def files as used in LambdaEngine, probably useful for other tasks. The basic design was inspired
by the record jar format from Eric S Raymond's art of UNIX programming.

Please look at test.def for an example file in the format.

# How to use
Include def_format.h in your program and see the comments in that file for more information. You must also statically link
to def_format.c.

Note that the library only deals with string formats, any numerical conversion must be done by the external program.

# Limitations and known issues
There is no attempt at optimisations (such as indexing etc) in this library - you can however trivially build your own on top.
If anyone comes up with a clean and portable way to do indexing please feel free to submit a pull request on github.
