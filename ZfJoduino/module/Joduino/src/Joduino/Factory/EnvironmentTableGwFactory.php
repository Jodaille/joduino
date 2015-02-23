<?php

namespace Joduino\Factory;

use Zend\ServiceManager\FactoryInterface;
use Zend\ServiceManager\ServiceLocatorInterface;

class EnvironmentTableGwFactory implements FactoryInterface
{
    public function createService(ServiceLocatorInterface $serviceLocator) {

      $dbAdapter = $serviceLocator->get('Zend\Db\Adapter\Adapter');

      $resultSetPrototype = new \Zend\Db\ResultSet\ResultSet();

      $resultSetPrototype->setArrayObjectPrototype(new \Joduino\Model\Environment());

      return new \Zend\Db\TableGateway\TableGateway('environment', $dbAdapter, null, $resultSetPrototype);
    }
}
