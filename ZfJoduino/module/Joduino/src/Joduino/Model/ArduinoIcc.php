<?php
/**
* ArduinoIcc.php
*
*/
namespace Joduino\Model;

use Zend\ServiceManager\ServiceLocatorAwareInterface;
use Zend\ServiceManager\ServiceLocatorInterface;

class ArduinoIcc implements ServiceLocatorAwareInterface
{
	protected $service_manager;
	private $sendIDeuxCcmd;
	
	public function __construct($service_manager)
	{
		$this->service_manager = $service_manager;
		$config = $this->getServiceLocator()->get('config');
		$sendIDeuxCcmd = $config['Joduino']['sendIDeuxCcmd'];

		if(!isset($sendIDeuxCcmd) OR is_null($sendIDeuxCcmd))
		{
			$e = new \Exception('Please add Joduino sendIDeuxCcmd to config !');
			throw $e;
		}
		
		$this->sendIDeuxCcmd = $sendIDeuxCcmd;
	}
	
	public function sendMsgToArduino($code, $decode = true)
	{
		$json = false;

		$cmd = $this->sendIDeuxCcmd . ' -m ';
	
		exec($cmd . " $code", $output);
		$brutJson = trim(current($output));

		if($decode)
			$json = json_decode($brutJson);
		else
			$json = $brutJson;

		switch (json_last_error()) {
			case JSON_ERROR_DEPTH:
				$error = ' - Profondeur maximale atteinte';
			break;
			case JSON_ERROR_STATE_MISMATCH:
				$error = ' - Inadéquation des modes ou underflow';
			break;
			case JSON_ERROR_CTRL_CHAR:
				$error = ' - Erreur lors du contrôle des caractères';
			break;
			case JSON_ERROR_SYNTAX:
				$error = ' - Erreur de syntaxe ; JSON malformé';
			break;
			case JSON_ERROR_UTF8:
				$error = ' - Caractères UTF-8 malformés, probablement une erreur d\'encodage';
			break;
			default:
	        	break;
		}

		return $json;	
	}
    public function setServiceLocator(ServiceLocatorInterface $serviceLocator)
    {
        $this->service_manager = $serviceLocator;
    }

    public function getServiceLocator()
    {
        return $this->service_manager;
    }
}