ZfJoduino
=======================

Introduction
------------
We have used the ZF2 skeleton application to build an interface.
This application allow to display datas received by Arduino,
and send to it a command code to execute action (switch on/off relays).

Installation
------------
You need to install ZendSkeletonApplication
cf: https://github.com/zendframework/ZendSkeletonApplication

There are some examples for databases structure in
ZfJoduino/vendor/jodaille/joduino/docs directory

Configuration
-------------
You have to add apache user to i2c group,
here user is www-data so we could add it as:

```bash
adduser www-data i2c
```

Crontab configuration
---------------------
cf: ZfJoduino/vendor/jodaille/joduino/docs/crontab_sample
