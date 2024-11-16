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

After installing PostgreSQL, you need to login to the user and create a database. Here is an example (if you are using the default user `postgres`, and created a database `tpch`):

```bash
psql -U postgres
```

```sql
CREATE DATABASE tpch;
```

You don't need to load the data into the database (or if you have done, it is better), because the project will download the data and load it into the database automatically.

## Run

A simple command to run the project:

```bash
sc3020
```

By default, the project will run on [`http://localhost:8000`](http://127.0.0.1:8000).
