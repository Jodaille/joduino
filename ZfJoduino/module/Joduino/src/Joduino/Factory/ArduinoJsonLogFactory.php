<?php

namespace Joduino\Factory;

use Zend\ServiceManager\FactoryInterface;
use Zend\ServiceManager\ServiceLocatorInterface;

class ArduinoJsonLogFactory implements FactoryInterface
{
    public function createService(ServiceLocatorInterface $serviceLocator) {

        $model = new \Joduino\Model\ArduinoJsonLog();
        $model->setServiceLocator($serviceLocator);
        return $model;
    }
}
