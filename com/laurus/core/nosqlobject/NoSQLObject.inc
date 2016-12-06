<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of NoSQLObject
 *
 * @author Amruta Vichare <amruta.vichare@laurus-it.com>
 */
class NoSQLObject {
    private $col;
    private $operation;
    private $data;
    private $db;
    public function getCol() {
        return $this->col;
    }

    public function setCol($col) {
        $this->col = $col;
    }

    public function getOperation() {
        return $this->operation;
    }

    public function setOperation($operation) {
        $this->operation = $operation;
    }

    public function getData() {
        return $this->data;
    }

    public function setData($data) {
        $this->data = $data;
    }

        public function getDb() {
        return $this->db;
    }

    public function setDb($db) {
        $this->db = $db;
    }

    


}

?>
