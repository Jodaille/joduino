<?php
/**
 * Joduino (https://github.com/Jodaille)
 *
 * @link      https://github.com/Jodaille/joduino for the canonical source repository
 * @copyright Copyright (c) 2014 Jodaille (http://jodaille.org)
 * @license   New BSD License
 */

return array(
    'Joduino' => array(
	'sendIDeuxCcmd' => 'python /home/pi/joduino/raspberry/testI2c.py',
	'foam' => array(
			'on' => '',
			'off' => ''
			),
	'phpSettings'   => array(
				'display_startup_errors'=> true,
				'display_errors' => true,
	        		'max_execution_time' => 60,
				'error_reporting', E_ALL,
    				),
    ),
    'router' => array(
        'routes' => array(
            'home' => array(
                'type' => 'Zend\Mvc\Router\Http\Literal',
                'options' => array(
                    'route'    => '/',
                    'defaults' => array(
                        'controller' => 'Joduino\Controller\Index',
                        'action'     => 'index',
                    ),
                ),
            ),
              'detecthttp' => array(
                'type' => 'Zend\Mvc\Router\Http\Literal',
                'options' => array(
                    'route'    => '/detecthttp',
                    'defaults' => array(
                        'controller' => 'Joduino\Controller\Index',
                        'action'     => 'detecthttp',
                    ),
                ),
            ),
             'detecti2c' => array(
                'type' => 'Zend\Mvc\Router\Http\Literal',
                'options' => array(
                    'route'    => '/detecti2c',
                    'defaults' => array(
                        'controller' => 'Joduino\Controller\Index',
                        'action'     => 'detecti2c',
                    ),
                ),
            ),
             'histo' => array(
                'type' => 'Zend\Mvc\Router\Http\Literal',
                'options' => array(
                    'route'    => '/histo',
                    'defaults' => array(
                        'controller' => 'Joduino\Controller\Index',
                        'action'     => 'histo',
                    ),
                ),
            ),
            // The following is a route to simplify getting started creating
            // new controllers and actions without needing to create a new
            // module. Simply drop new controllers in, and you can access them
            // using the path /application/:controller/:action
            'joduino' => array(
                'type'    => 'Literal',
                'options' => array(
                    'route'    => '/joduino',
                    'defaults' => array(
                        '__NAMESPACE__' => 'Joduino\Controller',
                        'controller'    => 'Index',
                        'action'        => 'index',
                    ),
                ),
                'may_terminate' => true,
                'child_routes' => array(
                    'default' => array(
                        'type'    => 'Segment',
                        'options' => array(
                            'route'    => '/[:controller[/:action]]',
                            'constraints' => array(
                                'controller' => '[a-zA-Z][a-zA-Z0-9_-]*',
                                'action'     => '[a-zA-Z][a-zA-Z0-9_-]*',
                            ),
                            'defaults' => array(
                            ),
                        ),
                    ),
                ),
            ),
        ),
    ),
    'service_manager' => array(
        'abstract_factories' => array(
            'Zend\Cache\Service\StorageCacheAbstractServiceFactory',
            'Zend\Log\LoggerAbstractServiceFactory',
        ),
        'aliases' => array(
            'translator' => 'MvcTranslator',
        ),
    ),
    'translator' => array(
        'locale' => 'fr_FR',
        'translation_file_patterns' => array(
            array(
                'type'     => 'gettext',
                'base_dir' => __DIR__ . '/../language',
                'pattern'  => '%s.mo',
            ),
        ),
    ),
    'controllers' => array(
        'invokables' => array(
            'Application\Controller\Index' => 'Application\Controller\IndexController',
            'Joduino\Controller\Index' => 'Joduino\Controller\IndexController',
            'Joduino\Controller\Cron' => 'Joduino\Controller\CronController',
        ),
    ),
    'view_manager' => array(
        'display_not_found_reason' => true,
        'display_exceptions'       => true,
        'doctype'                  => 'HTML5',
        'not_found_template'       => 'error/404',
        'exception_template'       => 'error/index',
        'template_map' => array(
            'layout/layout'           => __DIR__ . '/../view/layout/layout.phtml',
            'application/index/index' => __DIR__ . '/../view/application/index/index.phtml',
            'joduino/index/histo' => __DIR__ . '/../view/joduino/index/histo.phtml',
            'joduino/index/detecti2c' => __DIR__ . '/../view/joduino/index/detecti2c.phtml',
            'joduino/index/detecthttp' => __DIR__ . '/../view/joduino/index/detecthttp.phtml',
            'history'                => __DIR__ . '/../view/joduino/index/partial/history.phtml',
            'error/404'               => __DIR__ . '/../view/error/404.phtml',
            'error/index'             => __DIR__ . '/../view/error/index.phtml',
        ),
        'template_path_stack' => array(
            __DIR__ . '/../view',
        ),
        'strategies' => array(
            'ViewJsonStrategy',
        ),
    ),
// /joduino/cron/data
    // Placeholder for console routes
    'console' => array(
        'router' => array(
            'routes' => array(
                'foam-state' => array(
                    'options' => array(
                        'route' => 'foam [--verbose|-v] <state>',
                        'defaults' => array(
                            'controller' => 'Joduino\Controller\Cron',
                            'action' => 'foam',
                        )
                    )
                ),
                'fan-state' => array(
                    'options' => array(
                        'route' => 'fan [--verbose|-v] <state>',
                        'defaults' => array(
                            'controller' => 'Joduino\Controller\Cron',
                            'action' => 'fan',
                        )
                    )
                ),
		'log-environment-route' => array(
		    'type'    => 'simple',       // <- simple route is created by default, we can skip that
		    'options' => array(
		        'route'    => 'logenvironment',
		        'defaults' => array(
		            'controller' => 'Joduino\Controller\Cron',
		            'action'     => 'logenvironment'
		        )
		    )
)
            ),
        ),
    ),
);
