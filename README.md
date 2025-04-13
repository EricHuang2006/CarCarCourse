# Sample Code for NTU Car Car Course Midterm Project

## Installation

### Conda

Installation guide for conda can be found [here](https://conda.io/projects/conda/en/latest/user-guide/install/index.html).

```bash
conda create -n car python=3.12
conda activate car
cd python
pip install -r requirements.txt
```

### Pip

```bash
cd python
pip install -r requirements.txt
```

## Miscellaneous
### Test Files
[能動的十字循跡](test/cross_test/cross_test.ino)
### Parameters
| Action |  Speed(left, right) | Duration(ms) |
|:---:|:---:|:---:|
| Right / Left Turn | (85, 255) | 650 |
| U-Turn | (225, -225) | 575 |
| Forward | (255, 255) | 200 |

### 焊接：

![alt text](image.png)