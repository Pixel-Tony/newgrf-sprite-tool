import argparse
import os
import string

template = '''// Automatically generated file, do not modify!
//
#include "{header}"

namespace {namespace}
{{
{content}
}}
'''
content_template = '{datatype} {name}{{.data_ = {encoded}, .size_ = {length}}};'
_printables = {
    *map(ord, {*string.printable} - {*string.whitespace, *'?\'"\\'})}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser() # TODO
    parser.add_argument('-H', '--header', required=True)
    parser.add_argument('-N', '--namespace', required=True)
    parser.add_argument('-D', '--datatype', required=True)
    parser.add_argument('-o', '--output', required=True)
    parser.add_argument('-i', '--input', required=True, action='append',
                        nargs=2)
    return parser.parse_args()


def encode_content(file_content: bytes):
    seq = [
        chr(byte) if byte in _printables else f'\\{oct(byte)[2:]:>03}'
        for byte in file_content
    ]
    seq_shortened = seq.copy()
    for i, (char, next_char) in enumerate(zip(seq, seq[1:])):
        if char == '\\000' and next_char.startswith('\\'):
            seq_shortened[i] = '\\0'

    CAP = 1000
    return '\n'.join(
        '"{}"'.format(''.join(seq_shortened[i:i + CAP]))
        for i in range(0, len(seq), CAP)
    )

def pack(input_file: str, name: str, output_file: str):
    with open(input_file, 'rb') as file:
        content = file.read()

    with open(output_file, 'w', encoding='utf8') as file:
        repr_ = encode_content(content)
        file.write(
            template.format(name=name, length=len(content), repr=repr_))


def main():
    arguments = parse_args()
    datatype = arguments.datatype
    contents = []
    for filename, var_name in arguments.input:
        with open(filename, 'rb') as f:
            fcontent = f.read()
        encoded = encode_content(fcontent)
        contents.append(
            content_template.format(
                datatype=datatype,
                name=var_name,
                encoded=encoded,
                length=len(fcontent)))

    result = template.format(
        header=arguments.header,
        namespace=arguments.namespace,
        content=os.linesep.join(contents))
    with open(arguments.output, 'w', encoding='utf8') as f:
        f.write(result)


if __name__ == '__main__':
    main()