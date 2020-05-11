#!/bin/bash
cp stockDB.service /etc/systemd/system/stockDB.service
systemctl enable stockDB.service
systemctl start stockDB.service
systemctl status stockDB.service
