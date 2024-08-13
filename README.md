How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-ststm32/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Change directory to example
$ cd platform-ststm32/examples/cmsis-blink

# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Build specific environment
$ pio run -e disco_f407vg

# Upload firmware for the specific environment
$ pio run -e disco_f407vg --target upload

# Clean build files
$ pio run --target clean
```

$$
\begin{aligned}
&\begin{cases}
\dot{x}_1=-ax_2\\
\dot{x}_2=-bx_1\\
\end{cases}\\
\Rightarrow&\begin{cases}
sX_1-x_1(0)=-aX_2\\
sX_2-x_2(0)=-bX_1\\
\end{cases}\\
\Rightarrow&\begin{cases}
sX_1+aX_2=x_1(0)\\
bX_1+sX_2=x_2(0)\\
\end{cases}\\
\Rightarrow&\begin{cases}
\begin{bmatrix}
s & a \\
b & s \\
\end{bmatrix}
\begin{bmatrix}
X_1 \\
X_2 \\
\end{bmatrix}=
\begin{bmatrix}
x_1(0) \\
x_2(0) \\
\end{bmatrix}
\end{cases}\\
\end{aligned}
$$
