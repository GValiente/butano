The common fonts are based on [Source Han Serif](https://github.com/adobe-fonts/source-han-serif) under [OFL-1.1 License](../../licenses/source-han-serif.txt).

![Source Han Sans Version 2 Specimen](https://upload.wikimedia.org/wikipedia/commons/0/0f/Source_Han_Sans_Version_2_Specimen.svg)

Include a font in your project's Makefile to use it.

2 choices to do that:

- include BMFonts

You need to install [Pillow](https://pillow.readthedocs.io/en/stable/installation.html) for BMFont support.

[Example](../../examples/text/Makefile)

- include graphics and headers

They are already converted for direct use in your project, so you needn't install anything.

[Example](https://github.com/laqieer/gba-dev-best-practice/blob/main/source/common-text/Makefile)

