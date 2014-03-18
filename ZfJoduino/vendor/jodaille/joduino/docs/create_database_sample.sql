--
-- Simple create in one line a database named: joduino
-- give privilege to user: joduinouser
-- with password: joduinopassword
-- @author Jodaille <raspberryjody@gmail.com>
--

CREATE database joduino;GRANT ALL PRIVILEGES ON joduino.* TO "joduinouser"@"localhost" IDENTIFIED BY "joduinopassword";FLUSH PRIVILEGES;

