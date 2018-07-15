# WordMIX - Train foreign language vocabulary by creating your own dictionary
[![Build Status](https://travis-ci.com/gchlebus/WordMIX.svg?branch=master)](https://travis-ci.com/gchlebus/WordMIX)
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

<p align="center">
  <img src ="Logo.png" />
</p>

## How it works

- Add words and their definitions to the dictionary.
- Memorize them using the **Learn mode**.

## Main window

![MainWindow](MainWindow.png)

## Learn mode

<p align="center">
  <img src ="LearnMode.gif" />
</p>

# Installation

## macOS

```
# Tested with Qt 5.11.1
brew install qt
export PATH="/usr/local/opt/qt/bin:$PATH"

git clone https://github.com/gchlebus/WordMIX.git
cd WordMIX/WordMIX
qmake && make && make install
```

