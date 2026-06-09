import sys

with open('src/gui/MainWindow.cpp', 'r', encoding='utf-8') as f:
    text = f.read()

text = text.replace('background-color: #f6f8fa;', 'background-color: #ffffff;')
text = text.replace('background: #f6f8fa;', 'background: #ffffff;')
text = text.replace('rgba(13, 17, 23, 180)', 'rgba(255, 255, 255, 200)')
text = text.replace('"  padding: 10px;"\n                         "}"', '"  padding: 10px;"\n                         "  background-color: #ffffff;"\n                         "}"')

with open('src/gui/MainWindow.cpp', 'w', encoding='utf-8') as f:
    f.write(text)
