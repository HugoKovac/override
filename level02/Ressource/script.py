import sys, codecs

result = ''

for i in range(1, len(sys.argv)):
    p = str(sys.argv[i]).replace('0x', '', 1)
    p_bytes = bytes(p, encoding='utf-8')
    p_str = codecs.decode(p_bytes, "hex")
    result += str(p_str[::-1], "utf-8")
    
print(result)