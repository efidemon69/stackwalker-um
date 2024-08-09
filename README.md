# Simple manual stack walker (usermode)
This project demonstrates a basic manual stack-walking.

## Features

- **Manual Stack Walking**: Traverses the call stack manually by following the base pointes (EBP).
- **Safe Memory Access**: Includes error handling to safely read memory and prevent crashes due to invalid memory access.
- **Stack Frame structure**: Uses a stackfrme struct to represent each stack frame for better code org
