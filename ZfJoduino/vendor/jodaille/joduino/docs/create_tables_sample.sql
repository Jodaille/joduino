--
-- Simple create table: joduino.environment
-- @author Jodaille <raspberryjody@gmail.com>
--
CREATE TABLE `environment` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sensor_id` int(3) DEFAULT NULL,
  `humidity` int(3) DEFAULT NULL,
  `temperature` varchar(6) DEFAULT NULL,
  `photoresistance` int(4) DEFAULT NULL,
  `msg` varchar(60) DEFAULT NULL,
  `msgdate` datetime DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB


