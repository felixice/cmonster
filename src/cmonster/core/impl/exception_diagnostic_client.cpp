/*
Copyright (c) 2011 Andrew Wilkins <axwalk@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "exception_diagnostic_client.hpp"

#include <llvm/Support/Compiler.h>
#include <llvm/ADT/SmallString.h>

namespace cmonster {
namespace core {
namespace impl {

ExceptionDiagnosticClient::ExceptionDiagnosticClient(
    boost::exception_ptr &exception) : m_exception(exception) {}

void
ExceptionDiagnosticClient::HandleDiagnostic(
    clang::DiagnosticsEngine::Level level, const clang::Diagnostic &info)
{
    llvm::SmallString<64> formatted;
    info.FormatDiagnostic(formatted);
    m_exception = boost::copy_exception(std::runtime_error(formatted.c_str()));
}

clang::DiagnosticConsumer*
ExceptionDiagnosticClient::clone(clang::DiagnosticsEngine &diags) const
{
    return new ExceptionDiagnosticClient(m_exception);
}

}}}

