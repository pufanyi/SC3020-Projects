# SC3020 Project

## Installation

You can either choose to build from source or build from pypi, we require python version to be at least 3.9

## Build from source
```bash
# Create a new conda environment
# Upgrade pip if necessary
# python3 -m pip install --upgrade pip
conda create --name sc3020 python=3.11
conda activate sc3020

# Install the package
python -m pip install -e .
# You can also run
# python -m pip install .
# to install it as a formal package
```

## Build from pypi
We have also prepared a pypi version of it, you can also download it simply using pip. The code version is the same as the code in GitHub.
```bash
python -m pip install sc3020-g6-2024==0.1.0
```

## Build with no-deps option
If you encountering any error when directly install the packages, you can also build the packages without any dependencies and then install the requirements from a txt file. We verified this environment version can successfully run the application
```bash
# Build only the sc3020 packages
python -m pip install --no-deps --no-cache-dir -e .

# Build the required requirements
python -m pip install -r requirements.txt
```


## Config PostgreSQL

PostgreSQL is required to run the project. You can download it from [here](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads).

After installing PostgreSQL, you need to login to the user and create a database.

Here is an example.

We assume the user is the default user `postgres`.

```bash
psql -U postgres
```

And then we create a database named `tpch`.

```sql
CREATE DATABASE tpch;
```

You don't need to load the data into the database (or if you have done, it is better), because the project will download the data and load it into the database automatically. Make sure the database is served at the backend so that the application can connect to the database.

## Run

A simple command to run the project:

```bash
# Use cli
sc3020

# Use python
python3 src/sc3020/project.py
# Or
python3 -m sc3020.project
# If you want to run the program as a python module
```

By default, the project will run on [`http://localhost:8000`](http://127.0.0.1:8000). You can then access `localhost:8000` once you started the server. The default index page is a welcome page. Click the `Go to Application` button to actual go to the application. You can also direct access the [`localhost:8000/sc3020`](http://localhost:8000/sc3020/) to access the project.
