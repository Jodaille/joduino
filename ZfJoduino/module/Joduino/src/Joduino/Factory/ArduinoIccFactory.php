<?php
// ZfJoduino/module/Joduino/src/Joduino/Factory/ArduinoIccFactory.php
namespace Joduino\Factory;

use Zend\ServiceManager\FactoryInterface;
use Zend\ServiceManager\ServiceLocatorInterface;

class ArduinoIccFactory implements FactoryInterface
{
    public function createService(ServiceLocatorInterface $serviceLocator) {

        $model = new \Joduino\Model\ArduinoIcc($serviceLocator);
        return $model;
    }
}
