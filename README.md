# rETL DB

[![Build Status](https://img.shields.io/badge/build-pending-yellow)](https://github.com/scottsbergeron/retldb)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

A specialized database designed for Reverse ETL scenarios, optimized for serving data warehouse data to online systems with low latency.

## Overview

rETL DB is a high-performance database system built in C that specializes in:

1. **Bulk data operations** - Optimized for full table or partition replacements rather than row-level mutations
2. **Fast point lookups** - Sub-millisecond response times for key-based queries
3. **Efficient analytical queries** - Fast aggregations and grouping operations

Unlike traditional databases, rETL DB is specifically designed for reverse ETL workflows where data is periodically synchronized from a data warehouse to serve online systems efficiently.

## Project Status

🚧 **Early Development** - This project is in the initial development phase (Phase 1: Core Engine and Storage).

## Features (Planned)

- Columnar storage format for efficient analytical queries
- Row-based indexes for fast point lookups
- Immutable data files with LSM-tree approach
- Partition-based storage for efficient bulk replacements
- Memory-mapped files for high-performance access
- Simple SQL interface for queries
- Optimized for read-heavy workloads
- C library with bindings for popular languages

## Building from Source

### Prerequisites

- C compiler (GCC, Clang, or MSVC)
- CMake (version 3.15+)
- LZ4 and Snappy compression libraries

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/scottsbergeron/retldb.git
cd retldb

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run tests
make test
```

## Documentation

Comprehensive documentation will be available once the initial development phase is complete.

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to get started.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by modern columnar databases and LSM-tree storage engines
- Built to address the specific needs of reverse ETL workflows
