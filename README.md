sphia-transactions
==================

A sphia plugin that allows transactions in sophiaDB

## Installation

with [clib](https://github.com/clibs) do,

```
```

with git do,

```
$ git clone git@github.com:deepak1556/sphia-transactions.git /tmp/sphia-transactions
$ make -C /tmp/sphia-transactions make install
```

## Usage

```
$ sphia-transaction --path /tmp/
sphia>

commands :
  -- begin                                 [starts the transaction]
  -- commit                                [indicates completion of transaction]
  -- set --key <key> --value <value>       
```
