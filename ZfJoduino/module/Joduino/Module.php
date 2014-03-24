<?php
/**
 * Joduino (https://github.com/Jodaille)
 *
 * @link      https://github.com/Jodaille/joduino for the canonical source repository
 * @copyright Copyright (c) 2014 Jodaille (http://jodaille.org)
 * @license   New BSD License
 */

namespace Joduino;

use Zend\Mvc\ModuleRouteListener;
use Zend\Mvc\MvcEvent;

use Joduino\Model\Environment;
use Joduino\Model\EnvironmentTable;
use Zend\Db\ResultSet\ResultSet;
use Zend\Db\TableGateway\TableGateway;

/** CLI part */
use Zend\ModuleManager\Feature\ConsoleBannerProviderInterface;
use Zend\ModuleManager\Feature\ConsoleUsageProviderInterface;
use Zend\Console\Adapter\AdapterInterface as Console;

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

    /**
     * This method is defined in ConsoleBannerProviderInterface
     */
    public function getConsoleBanner(Console $console)
    {
        return 'Joduino';
    }

    /**
     * This method is defined in ConsoleUsageProviderInterface
     */
    public function getConsoleUsage(Console $console)
    {
        return array(
            'foam [--verbose|-v] <state>' => 'Change foam state.',
            'fan [--verbose|-v] <state>' => 'Change fan state.',
        );
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
