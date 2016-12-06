<?php

import('com.laurus.core.connection.IConnectionProperties');
import('com.laurus.core.connection.IConn');

/**
 * @name MongoDbConnection
 * @author Amruta Vichare
 */
class MongoDbConnection implements IConn {

    protected $logger;
    private static $CONFIG_FILE = "mongodb.json";
    private static $CFG_FOLDERNAME = "cfg";
    private $arr;
    private $operArr;

    public function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
        $this->arr = $this->readConfigurationFile();
        $this->operArr = $this->getMongoOperations();
    }

    public function createConnection($confName) {
        try {
            $this->logger->debug($confName);
            $this->logger->debug($this->arr[$confName]);
            $this->port = $this->arr[$confName]["DATABASE_PORT"];
            $this->mongocon = $this->connect($this->arr[$confName]["DATABASE_SERVER"], $this->arr[$confName]["DATABASE_USER"], $this->arr[$confName]["DATABASE_PASSWORD"], $this->arr[$confName]["DATABASE_NAME"]);
            return $this->mongocon;
        } catch (\MongoConnectionException $ex) {
            throw new Exception("Could not connect to database server", $ex->getCode());
        }
    }

    public function execute(\NoSQLObject $obj) {
        $this->logger->debug("executesdasdasd");
        $col = $obj->getCol();
        $operation = $obj->getOperation();
        $data = $obj->getData();
        $arr = $this->operArr;
        $this->logger->debug($arr);
        $this->logger->debug($arr[$operation]);
        $this->logger->debug($data);
        if ($arr[$operation] != null) {
            $this->logger->debug("inside");
//            if(function_exists($arr[$operation])){
            $this->logger->debug("is present");
            return $this->$arr[$operation]($col, $data);
//            }
        }
    }

    public function executeOwnQuery(\NoSQLObject $obj) {
        $this->logger->debug($obj);
        $col = $obj->getCol();
        $operation = $obj->getOperation();
        $data = $obj->getData();
        $res = call_user_func_array(array($this->db->$col, $operation), $data);
        $this->logger->debug($operation);
        $this->logger->debug($data);

        $this->logger->debug($res);
        return $res;
    }

    public function executeRunCommand(\NoSQLObject $obj) {
        $this->logger->debug($obj);
        $col = $obj->getCol();
        $operation = $obj->getOperation();
        $data = $obj->getData();
        $arr = $this->operArr;
        $this->logger->debug($operation);
        $this->logger->debug($arr[$operation]);
        if ($arr[$operation] != null) {
            $this->logger->debug("inside");
//            if(function_exists($arr[$operation])){
            $this->logger->debug("is present");
            return $this->$arr[$operation]($col, $data);
//            }
        }
    }

    public function executeFunction(\NoSQLObject $obj) {
        $this->logger->debug($obj);
        $col = $obj->getCol();
        $data = $obj->getData();
        $response=$this->db->execute($data);
        return $response['retval'];
    }

    private function insertRunCommand($col, $data) {
        $this->logger->debug("insertRunCommand");
        $insideData = array("insert" => $col, "documents" => $data);
        $this->logger->debug($insideData);
        $res = $this->command($insideData);
        $this->logger->debug($res);
        return $res;
    }

    private function deleteRunCommand($col, $data) {
        $this->logger->debug("deleteRunCommand");
        $insideData = array("delete" => $col, "deletes" => $data);
        $this->logger->debug($insideData);
        $res = $this->command($insideData);
        $this->logger->debug($res);
        return $res;
    }
    
    public function command($data){
        return $this->db->command($data);
    }

    private function updateRunCommand() {
        
    }

    private function update($col, $data) {
        return $this->db->$col->update($data);
    }
    
    private function remove($col, $data) {
        return $this->db->$col->remove($data);
    }

    private function select($col, $data = null) {
        $this->logger->debug($col);
        $this->logger->debug($this->db);
        $res = $this->db->$col->find();
        $this->logger->debug($res);
        return $res;
    }

    private function insert($col, $data) {
        return $this->db->$col->insert($data);
    }

    private function findWithSort($col, $data) {
        $this->logger->debug("inside find with sort");
        if ($data['sort']) {
            return $this->db->$col->find($data['query'])->sort($data['sort']);
        } else {
            return $this->db->$col->find($data['query']);
        }
    }

    private function findWithLimit($col, $data) {
        $this->logger->debug("inside find with sort");
        if ($data['limit']) {
            return $this->db->$col->find($data['query'])->limit($data['limit']);
        } else {
            return $this->db->$col->find($data['query']);
        }
    }

    private function findWithLimitSkip($col, $data) {
        $this->logger->debug("inside find with limit skip");
        $this->logger->debug($data['limit']);
        $this->logger->debug($data['skip']);
        if ($data['limit']) {
            $this->logger->debug("inside if");
            return $this->db->$col->find($data['query'])->limit($data['limit'])->skip($data['skip']);
        } else {
            return $this->db->$col->find($data['query']);
        }
    }

    private function drop($col) {
        return $this->db->$col->drop();
    }

    public function connect(/* String */ $hostname, /* String */ $username, /* String */ $password, /* String */ $db) {

        $this->logger->debug($db);
        if ($username != null) {
            $this->mongoConnection = new MongoClient("mongodb:" . $username . ":" . $password . "@" . $hostname . ":" . $this->port . "/" . $db);
        } else if ($username == null && $password == null) {
            $this->mongoConnection = new MongoClient("mongodb://" . $hostname . ":" . $this->port . "/" . $db);
            // $this->mongoConnection = new MongoClient();
        }

        $this->db = $this->mongoConnection->$db;
        $this->logger->debug($this->db);
        return $this->mongoConnection;
    }

    private function readConfigurationFile() {
        $pageConfigFileName = $this->getFilePath();
        if (file_exists($pageConfigFileName)) {
            $arr = json_decode(file_get_contents($this->getFilePath()), true);
        } else {
            $this->logger->warn($pageConfigFileName . "mongodb.json not found in the application");
            throw new Exception("mongodb.json not found in the application");
        }
        return $arr;
    }

    private function getFilePath() {
        return $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME . DIRECTORY_SEPARATOR . self::$CONFIG_FILE;
    }

    public function close() {
        
    }
    
    public function dropCollection(\NoSQLObject $nosqlobj){
        $col=$nosqlobj->getCol();
        $collection = $this->db->$col;
        $response = $collection->drop();
        return $response;
    }

    private function getMongoOperations() {
        return array("insert" => "insert", "update" => "update", "select" => "select", "remove" => "remove", "insertRunCommand" => "insertRunCommand", "findwithsort" => "findWithSort", "findwithlimit" => "findWithLimit", "findwithlimitskip" => "findWithLimitSkip");
    }

}

?>
