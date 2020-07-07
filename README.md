# base64encoder
Simple console application to convert Ascii to base 64 format in c++.

Intended to provide a way to convert ascii text to base64 format and viceversa.

Options are:

 -d: This means "decoding". When specified in the console parameters, the application is instructed to work as a base64 decoder. If this is not specified the encoding mode is set.

-sT: Source text. Following this option should be the text to be encoded or decoded from/to base64 format.

-sF: Source file. Following this option should be the file to be read for encoding or decoding from/to base64 format.

-dF: Destination file. Informs the application to save the result into a file that is specified next to this option.

Example (using the executable in the location it has been compiled)
----------------------------
a) Encode ascii text to base64 text:
----------------------------
Base64Encoder.exe -sT Hello

Result:

base64 encoding

SGVsbG8=

----------------------------
b) Decode base64 text to ascii text:
----------------------------
Base64Encoder.exe -sT SGVsbG8= -d

Result:

base64 decoding

Hello

----------------------------
c) Encode ascii text to base64 file text:
----------------------------
> Base64Encoder.exe -sT Hello -dF "c:\temp\base64.txt"

Result:

base64 encoding

text encoded to file "c:\temp\base64.txt"

----------------------------
d) Decode base64 file to ascii text:
----------------------------
Base64Encoder.exe -sF "c:\temp\base64.txt" -d  

Result:

base64 decoding

Hello

Note: is this case as no option was specified for destination it is assumed "-dT" (destination text), so decoding is printed on screen.


