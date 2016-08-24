<?hh // strict
use Exception;
namespace Decouple\Test;
class TestCase {
  private int $assertions = 0;

  public function assertEquals(mixed $a, mixed $b): void {
    if ($a != $b) {
      try {
        if (gettype($a) == 'object') {
          $a = get_class($a);
        }
        if (gettype($b) == 'object') {
          $b = get_class($b);
        }
        throw new TestException(
          sprintf(
            "Unable to assert that %s == %s",
            (string) $a,
            (string) $b,
          ),
        );
      } catch (Exception $e) {
        throw new TestException("Failed assertion");
      }
    }
    $this->assertions++;
  }

  public function assertEqualsExactly(mixed $a, mixed $b): void {
    if ($a !== $b) {
      throw new TestException(
        sprintf(
          "Unable to assert that %s === %s",
          (string) $a,
          (string) $b,
        ),
      );
    }
    $this->assertions++;
  }

  public function assertNotEquals(mixed $a, mixed $b): void {
    if ($a == $b) {
      throw new TestException(
        sprintf(
          "Unable to assert that %s != %s",
          (string) $a,
          (string) $b,
        ),
      );
    }
    $this->assertions++;
  }

  public function assertions(): int {
    return $this->assertions;
  }
}
