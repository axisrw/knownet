<?php
require_once("../libcdn2.inc");
$cdn2 = new CDN2SoapClient("http://localhost:8080/transcode-web-1.0-SNAPSHOT/services/transcodeService?wsdl", 
                           "55cf032b-8997-11dd-a207-bf051f0cf6bd", "OGFiOWNmMDYtNTNkNy0zZWJkLWFjYmQtMzg5ZTFhOTIyM2M0");
print_r($cdn2->getVideoToken());
?>