<?php
/**
* Joduino (https://github.com/Jodaille)
*
* @link      https://github.com/Jodaille/joduino for the canonical source repository
* @copyright Copyright (c) 2014 Jodaille (http://jodaille.org)
* @license   New BSD License
*/

namespace Joduino\Model;

use Zend\Db\TableGateway\TableGateway;
use Zend\Db\TableGateway\AbstractTableGateway;
use Zend\Db\Sql\Select;
class EnvironmentTable extends AbstractTableGateway
{
  protected $tableGateway;

  public function __construct(TableGateway $tableGateway)
  {
    $this->tableGateway = $tableGateway;
  }

  public function fetchAll()
  {
    $resultSet = $this->tableGateway->select();
    return $resultSet;
  }

  public function getHistoBar($type = 'temperature')
  {
    $this->adapter = $this->tableGateway->getAdapter();

    $sql = "SELECT $type,
    COUNT(*) AS COUNT,
    RPAD('', LN(COUNT(*)), '*') AS bar
    FROM
    environment
    WHERE $type IS NOT NULL
    GROUP BY $type";

    $statement = $this->adapter->query($sql);
    $historyResults = $statement->execute();
    $history = array();

    foreach($historyResults as $h)
    {
      if(!is_null($h[$type]))
      $history[$h[$type]] = array(
        'bar' => $h['bar'],
        'count' => $h['COUNT'],
      );
    }
    return $history;
  }

  public function getHistory($nbelt = 20)
  {
    $resultSet = $this->tableGateway->select(
      function (Select $select) use ($nbelt) {
        $select->where('msgdate >= CURDATE()');

        if($nbelt)
        {
          $nbelt = (int) $nbelt;
          $select->order('msgdate DESC')->limit($nbelt);
        }
        else
        {
          $select->order('msgdate DESC');
        }
      }
    );
    return $resultSet;
  }

  public function getEnvironment($id)
  {
    $id  = (int) $id;
    $rowset = $this->tableGateway->select(array('id' => $id));
    $row = $rowset->current();
    if (!$row) {
      throw new \Exception("Could not find row $id");
    }
    return $row;
  }

  public function saveEnvironment(Environment $environment)
  {
    $data = array(
      'sensor_id' => $environment->sensor_id,
      'humidity' => $environment->humidity,
      'temperature' => $environment->temperature,
      'photoresistance' => $environment->photoresistance,
      'waterlevel' => $environment->waterlevel,
      'msgsent' => $environment->msgsent,
      'msgrcved' => $environment->msgrcved,
      'msgdate' => $environment->msgdate,
    );

    $id = (int) $environment->id;
    if ($id == 0) {
      $this->tableGateway->insert($data);
    } else {
      if ($this->getEnvironment($id)) {
        $this->tableGateway->update($data, array('id' => $id));
      } else {
        throw new \Exception('Environment id does not exist');
      }
    }
  }


  public function deleteEnvironment($id)
  {
    $this->tableGateway->delete(array('id' => (int) $id));
  }
}
