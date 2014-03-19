<?php
/**
 * Zend Framework (http://framework.zend.com/)
 *
 * @link      http://github.com/zendframework/ZendSkeletonApplication for the canonical source repository
 * @copyright Copyright (c) 2005-2014 Zend Technologies USA Inc. (http://www.zend.com)
 * @license   http://framework.zend.com/license/new-bsd New BSD License
 */

namespace Joduino\Controller;

use Zend\Mvc\Controller\AbstractActionController;
use Zend\Console\Request as ConsoleRequest;

use Zend\View\Model\ViewModel;
use Zend\View\Model\JsonModel;

class CronController extends AbstractActionController
{
	protected $environmentTable;

	public function foamAction()
	{
		$request = $this->getRequest();
		
		if (!$request instanceof ConsoleRequest){
		    throw new \RuntimeException('You can only use this action from a console!');
		}
		$verbose     = $request->getParam('verbose', false);
        	$state   = $request->getParam('state');

		$arduinoIcc = $this->getServiceLocator()->get('Joduino\Model\ArduinoIcc');
		$arduinoIcc->changeFoamState($state);

		return "foam $state!\n";
	}

	public function fanAction()
	{
		$request = $this->getRequest();
		
		if (!$request instanceof ConsoleRequest){
		    throw new \RuntimeException('You can only use this action from a console!');
		}
		$verbose     = $request->getParam('verbose', false);
        	$state   = $request->getParam('state');

		$arduinoIcc = $this->getServiceLocator()->get('Joduino\Model\ArduinoIcc');
		$arduinoIcc->changeFanState($state);

		return "fan $state!\n";
	}

	public function logenvironmentAction()
	{
		$arduinoIcc = $this->getServiceLocator()->get('Joduino\Model\ArduinoIcc');
		
		$json = $arduinoIcc->sendMsgToArduino(10, false);
		
		return "$json\n";
    	}

	public function indexAction()
    	{
		$arduinoIcc = $this->getServiceLocator()->get('Joduino\Model\ArduinoIcc');
		$foam 	= $this->getRequest()->getQuery('foam');
		$fan 	= $this->getRequest()->getQuery('fan');
	
		if($fan == 'fan_on')
		{
			$json = $arduinoIcc->sendMsgToArduino(11);
		}
		else
		{
			$json = $arduinoIcc->sendMsgToArduino(12);		
		}
	
		if($foam == 'foam_on')
		{
			$json = $arduinoIcc->sendMsgToArduino(14);
		}
		else
		{
			$json = $arduinoIcc->sendMsgToArduino(15);
		}
	
        	return new ViewModel(array('data' => $json, 'fan' => $fan, 'foam' => $foam));
    	}

	public function getEnvironmentTable()
	{
		if (!$this->environmentTable) {
			$sm = $this->getServiceLocator();
			$this->environmentTable = $sm->get('Joduino\Model\EnvironmentTable');
		}
		return $this->environmentTable;
	}
}
