# SC3020 Project

## Installation

```bash
# Create a new conda environment
conda create --name sc3020 python=3.11
conda activate sc3020

# Install the package
python -m pip install -e .
```

## Download PostgreSQL

PostgreSQL is required to run the project. You can download it from [here](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads).

## Run

```bash
sc3020
```

## Test

```bash
python test/run_suite.py
```
