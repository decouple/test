<?hh // strict
namespace Decouple\Test;
use Exception;
class TestException extends Exception {
  public function __construct(string $message) {
    $message = sprintf("[Assertion Failure] %s", $message);
    parent::__construct($message);
  }
}
