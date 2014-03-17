<?php
/**
 * Zend Framework (http://framework.zend.com/)
 *
 * @link      http://github.com/zendframework/ZendSkeletonApplication for the canonical source repository
 * @copyright Copyright (c) 2005-2014 Zend Technologies USA Inc. (http://www.zend.com)
 * @license   http://framework.zend.com/license/new-bsd New BSD License
 */

namespace Joduino;

use Zend\Mvc\ModuleRouteListener;
use Zend\Mvc\MvcEvent;

 use Joduino\Model\Environment;
 use Joduino\Model\EnvironmentTable;
 use Zend\Db\ResultSet\ResultSet;
 use Zend\Db\TableGateway\TableGateway;

class Module
{
    public function onBootstrap(MvcEvent $e)
    {
        $eventManager        = $e->getApplication()->getEventManager();
        $moduleRouteListener = new ModuleRouteListener();
        $moduleRouteListener->attach($eventManager);

	/** PHP settings */
        $application = $e->getApplication();
        $config      = $application->getConfig();

        $phpSettings = $config[__NAMESPACE__]['phpSettings'];

        if($phpSettings) {
            foreach($phpSettings as $key => $value) {
                ini_set($key, $value);
            }
        }
    }

    public function getServiceConfig()
    {
        return array(
            'factories' => array(
                'Joduino\Model\ArduinoIcc' => function ($sm) {
                    $ArduinoIcc = new \Joduino\Model\ArduinoIcc($sm);
                    $ArduinoIcc->setServiceLocator($sm);
                    return $ArduinoIcc;
                },
                 'Joduino\Model\EnvironmentTable' =>  function($sm) {
                     $tableGateway = $sm->get('EnvironmentTableGateway');
                     $table = new EnvironmentTable($tableGateway);
                     return $table;
                 },
                 'EnvironmentTableGateway' => function ($sm) {
                     $dbAdapter = $sm->get('Zend\Db\Adapter\Adapter');
                     $resultSetPrototype = new ResultSet();
                     $resultSetPrototype->setArrayObjectPrototype(new Environment());
                     return new TableGateway('environment', $dbAdapter, null, $resultSetPrototype);
                 },
            ),
        );
    }

    public function getConfig()
    {
        return include __DIR__ . '/config/module.config.php';
    }

    public function getAutoloaderConfig()
    {
        return array(
            'Zend\Loader\StandardAutoloader' => array(
                'namespaces' => array(
                    __NAMESPACE__ => __DIR__ . '/src/' . __NAMESPACE__,
                ),
            ),
        );
    }
}
