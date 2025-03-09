# Contributing to rETL DB

Thank you for your interest in contributing to rETL DB! This document provides guidelines and instructions for contributing to this project.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/scottsbergeron/retldb.git`
3. Create a new branch for your feature or bugfix: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Run tests to ensure your changes don't break existing functionality
6. Commit your changes with a descriptive commit message
7. Push to your branch: `git push origin feature/your-feature-name`
8. Open a Pull Request against the main repository

## Development Environment Setup

### Prerequisites

- C compiler (GCC, Clang, or MSVC)
- CMake (version 3.15+)
- LZ4 and Snappy compression libraries
- Google Test framework for unit testing

### Building the Project

```bash
mkdir build && cd build
cmake ..
make
```

### Running Tests

```bash
cd build
make test
```

## Coding Standards

- Use C99 standard
- Follow the project's coding style (see [CODING_STYLE.md](docs/CODING_STYLE.md))
- Write clear, concise comments
- Document all public APIs using Doxygen-style comments
- Include unit tests for new functionality

## Pull Request Process

1. Ensure your code follows the project's coding standards
2. Update documentation if necessary
3. Add or update tests as appropriate
4. Make sure all tests pass
5. The PR should clearly describe the problem and solution
6. Include the relevant issue number if applicable

## Reporting Bugs

- Use the GitHub issue tracker
- Describe the bug in detail
- Include steps to reproduce
- Mention your environment (OS, compiler version, etc.)
- If possible, provide a minimal code example that reproduces the issue

## Feature Requests

- Use the GitHub issue tracker
- Clearly describe the feature and its use case
- Explain how the feature would benefit the project

## Commit Guidelines

- Use clear, descriptive commit messages
- Reference issue numbers in commit messages when applicable
- Keep commits focused on a single logical change
- Squash multiple commits if they represent a single logical change

## License

By contributing to rETL DB, you agree that your contributions will be licensed under the project's MIT License. 