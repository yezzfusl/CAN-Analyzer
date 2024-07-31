# Can-Analyzer

**Can-Analyzer** is a sophisticated CAN bus analyzer developed in C. It provides real-time CAN data capture, filtering, decoding, and more. This tool is designed to help analyze and visualize CAN communication efficiently.

## Features

- [x] Basic CAN Communication
- [x] CAN Message Filtering
- [x] CAN Message Decoding
- [x] Real-time Data Display
- [x] Error Detection and Handling
- [x] Statistical Analysis Tools
- [x] Graphical Representation of Data

## Getting Started

### Prerequisites

- Linux-based operating system
- SocketCAN library (for CAN communication)
- Required development tools (gcc, make, build essential)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yezzfusl/CAN-Analyzer.git

2. Navigate to the project directory:
  ```cd CAN-Analyzer```
3. Build the project using `Makefile`:
  ```make```
## Usage
To use the CAN Analyzer, run:
   ```./can_analyzer <CAN interface>```
Replace `<CAN interface>` with the name of your CAN interface (e.g., can0).
Example:
   ```./can_analyzer can0```

## Output

The program provides the following outputs:

1. Real-time display of CAN messages in the terminal
2. Statistical analysis of CAN traffic, updated every second
3. A PNG file named `can_data.png` with a time-series graph of CAN message values, updated every second

## Project Structure

- `src/`: Contains all source files
- `include/`: Contains header files
- `Makefile`: Build script for the project
- `README.md`: This file, containing project documentation

## Key Components

1. `can_interface`: Handles CAN socket initialization and communication
2. `can_filter`: Implements CAN message filtering
3. `can_decode`: Decodes CAN messages into human-readable format
4. `real_time_display`: Manages the real-time display of CAN messages
5. `statistical_analysis`: Performs statistical analysis on CAN traffic
6. `graphical_display`: Generates graphical representations of CAN data
7. `error_handling`: Handles errors and exceptions

## Customization

- To modify the CAN message filter, edit the `filter` structure in `main.c`
- To change the graphing interval, modify the time comparison in the main loop of `main.c`
- To add new decoding rules, edit the `decode_can_message` function in `can_decode.c`

## Testing

The project has been tested with various CAN devices and simulators. To conduct your own tests:

1. Set up a virtual CAN interface:
   - sudo modprobe vcan
   - sudo ip link add dev vcan0 type vcan
   - sudo ip link set up vcan0

2. Use `cansend` to send test messages:
   ```./can_analyzer vcan0```
   
## Known Limitations

- The current implementation supports Standard CAN (11-bit identifiers) only
- The graphical display is limited to the first byte of each CAN message

## Future Improvements

- Add support for Extended CAN (29-bit identifiers)
- Implement more sophisticated decoding rules
- Create a GUI for easier interaction and data visualization

## Contributing

Contributions to this project are welcome. Please follow these steps:

1. Fork the repository
2. Create a new branch for your feature
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.
