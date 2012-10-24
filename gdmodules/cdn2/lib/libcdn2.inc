<?php
// $Id: libcdn2.inc,v 1.3.2.3 2009/01/02 01:14:11 akalsey Exp $

/*
 * @file libcdn2.inc
 */

define("CDN2_SHA1_BLOCKSIZE", 64);

class ErrorHandlingSoapClient {
  private $soapImpl;

  public function __construct($soapImpl) {
    $this->soapImpl = $soapImpl;
  }

  public function __call($function, $args) {
    try {
      return call_user_func_array(array($this->soapImpl, $function), $args);
    } catch (Exception $e) {
      return array('error' => $e->getMessage());
    } 
  }
}

interface CDN2SoapClient {
  public function getVideoToken($formats);
  public function getFormatPresets();
  public function getAssetsForVideoToken($videoToken);
  public function getVideosForSite();
  public function getVideoUploadStatus($videoToken);
  public function reencodeVideo($videoToken, $formats = array());
  public function deleteVideo($videoToken);
}

class CDN2SoapClientImpl implements CDN2SoapClient {
  static $instance;
  private $_siteToken;
  private $_secretKey;
  private $_endpointUrl;

  public static function getInstance($endPointUrl, $siteToken, $secretKey) {
    if (self::$instance == NULL) {
            self::$instance = new CDN2SoapClientImpl($endPointUrl, $siteToken, $secretKey);
    }
    return self::$instance; 
  }
  
  private function CDN2SoapClientImpl($endPointUrl, $siteToken, $secretKey) {
    $this->_endPointUrl = $endPointUrl;
    $this->_siteToken = $siteToken;
    $this->_secretKey = $secretKey;
    $this->_soapClient = new SoapClient($this->_endPointUrl, array('classmap' => array('authenticationToken' => 'CDN2AuthenticationToken'), 'trace' => 1));
    
  }

  public function getVideoToken($formats) {
    return $this->_soapClient->getVideoToken(array('authenticationToken' => $this->createDigestAuthenticationToken(), 'siteToken' => $this->_siteToken, 'formats' => $formats))->return;
  }

  public function getFormatPresets() {
    $result = $this->_soapClient->getFormatPresets(array('authenticationToken' => $this->createDigestAuthenticationToken()))->return;
    return $result;
  }

  public function getAssetsForVideoToken($videoToken) {
    return $this->_soapClient->getAssetsForVideoToken(array('authenticationToken' => $this->createDigestAuthenticationToken(), 'videoToken' => $videoToken))->return;
  }

  public function getVideosForSite() {
    return $this->_soapClient->getVideosForSite(array('authenticationToken' => $this->createDigestAutheticationToken()))->return;
  }
  public function getVideoUploadStatus($videoToken) {
    return $this->_soapClient->getVideoUploadStatus(array('authenticationToken' => $this->createDigestAuthenticationToken(), 'videoToken'=> $videoToken))->return;
  }

  public function reencodeVideo($videoToken, $formats = array()) {
    return $this->_soapClient->reencodeVideo(array('authenticationToken' => $this->createDigestAuthenticationToken(), 'videoToken' => $videoToken, 'formats' => $formats))->return;
  }

  public function deleteVideo($videoToken) {
    return $this->_soapClient->reencodeVideo(array('authenticationToken' => $this->createDigestAuthenticationToken(), 'videoToken' => $videoToken))->return;
  }

  public function hmacHash($key, $data) {
    $blocksize=64;
    $hashfunc='sha1';
    if (strlen($key)>$blocksize) {
      $key=pack('H*', $hashfunc($key));
    }
    $key=str_pad($key,$blocksize,chr(0x00));
    $ipad=str_repeat(chr(0x36),$blocksize);
    $opad=str_repeat(chr(0x5c),$blocksize);
    $hmac = pack(
      'H*',$hashfunc(
        ($key^$opad).pack(
          'H*',$hashfunc(
            ($key^$ipad).$data
          )
        )
      )
    );
    return base64_encode($hmac);
  }

  private function createDigestAuthenticationToken() {
    $timeStamp = date(DATE_RFC3339);

    $calculatedHash = $this->hmacHash($this->_secretKey, $this->_siteToken . $timeStamp);
    $authToken = new CDN2AuthenticationToken();
    $authToken->siteToken = $this->_siteToken;
    $authToken->timeStamp = $timeStamp;
    $authToken->calculatedHash = $calculatedHash;

    return new SoapVar($authToken, SOAP_ENC_OBJECT, 'cdn2AuthenticationToken', 'http://xml.radiantcdn.com/transcode');
  }
}

class CDN2AuthenticationToken {
  public $siteToken;
  public $timeStamp;
  public $calculatedHash;
}

