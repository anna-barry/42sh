incdirs = [
  include_directories('.')
]

common = [
  './lexer.c',
  './token.c',
]

token_printer = executable(
    # name of the output executable
    'token_printer',
    # source files for the executable
    common + [ './token_printer.c' ],
    # these are passed as -I
    include_directories: incdirs
)
