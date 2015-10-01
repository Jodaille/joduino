<?php

namespace Joduino\Factory;

use Zend\ServiceManager\FactoryInterface;
use Zend\ServiceManager\ServiceLocatorInterface;

class EnvironmentTableFactory implements FactoryInterface
{
    public function createService(ServiceLocatorInterface $serviceLocator) {

      $tableGateway = $serviceLocator->get('EnvironmentTableGateway');

      $table = new \Joduino\Model\EnvironmentTable($tableGateway);

      return $table;
    }
}
