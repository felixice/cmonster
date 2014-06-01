# cmonster

This is a fork of [cmonster](https://github.com/axw/cmonster).

For now, the only improvement is that this version does compile with the
latest clang and llvm sources. There is more to come, check the issues
(you can also open feature requests, they will be considered).

**Warning**: So far the refactorizations for the ~2 years of compatibility loss
are not tested at all, there might be bugs.

The following are the original readme contents.

cmonster is a Python wrapper for the [Clang](http://clang.llvm.org/) C++
parser.

As well as providing standard preprocessing/parsing capabilities, cmonster
adds support for:

* Inline Python macros;
* Programmatic #include handling; and
* (Currently rudimentary) source-to-source translation.

## Documentation

There's no proper documentation yet; I'll get to it eventually. In the mean
time...

### Inline Python macros

You can define inline Python macros like so:

```python
py_def(function_name(arg1, arg2, *args, **kwdargs))
    return [list_of_tokens]
py_end
```

Python macros must return a string, which will be tokenized, or a sequence of
tokens (e.g. some combination of the input arguments). Python macros are used
exactly the same as ordinary macros. e.g.

```python
py_def(REVERSED(token))
    return "".join(reversed(str(token)))
py_end
```
```cpp
int main() {
    printf("%s\n", REVERSED("Hello, World!"));
    return 0;
}
```

### Source-to-source translation

Source-to-source translation involves parsing a C++ file, generating an AST;
then traversing the AST, and making modifications in a _rewriter_. The rewriter
object maintains a history of changes, and can eventually be told to dump the
modified file to a stream.

For example, here's a snippet of Python code that shows how to make an insertion
at the top of each top-level function declaration's body:

```python
import cmonster
import cmonster.ast
import sys

# Create a parser, and a rewriter. Parse the code.
parser = cmonster.Parser(filename)
ast = parser.parse()
rewriter = cmonster.Rewriter(ast)

# For each top-level function declaration, insert a statement at the top of
# its body.
for decl in ast.translation_unit.declarations:
    if decl.location.in_main_file and \
       isinstance(decl, cmonster.ast.FunctionDecl):
        insertion_loc = decl.body[0]
        rewriter.insert(insertion_loc, 'printf("Tada!\\n");\n')

# Finally, dump the result.
rewriter.dump(sys.stdout)
```

## Installation

cmonster requires [Python 3.2](http://python.org/download/releases/3.2.2/),
[LLVM/Clang 3.0](http://llvm.org/releases/download.html#3.0), so first
ensure you have them installed. To build and install cmonster from source,
you will also need to install [Cython](http://cython.org/#download).

Now you can use
[easy\_install](http://packages.python.org/distribute/easy_install.html) to
install cmonster, like so: `easy_install cmonster`. This will download and
install a prebuilt binary distribution of cmonster.

If you wish to build cmonster yourself, either pull down the git repository, or
grab the source distribution from
[cmonster's PyPI page](http://pypi.python.org/pypi/cmonster/). When building,
make sure you have LLVM 3.0's llvm-config in your execution path. To verify
this, run `llvm-config --version`; you should expect to see `3.0` output. To
build from source, simply run `python3.2 setup.py install`.

