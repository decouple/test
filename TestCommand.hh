<?hh // partial
namespace Decouple\Test;
use Decouple\CLI\Command\CommandInterface;
use Decouple\CLI\Command\AwaitableCommand;
use Decouple\CLI\Console;
use Decouple\CLI\App;
use Decouple\Registry\Paths;
class TestCommand extends AwaitableCommand {
  public function __construct(private Paths $paths, private App $app) {
  }
  static function getName() : string {
    return 'test';
  }
  public async function execute() : Awaitable<void> {
    $config = $this->app->getConfig('cli/tests');
    if(!$config) {
      throw new \Exception("Invalid tests config");
    }
    $tests = $config->map();
    $test_dir = (string)$this->paths->get('test');
    Console::output(sprintf('Running %s tests in %s', count($tests), $test_dir));
    $passed = 0;
    $assertions = 0;
    $total = count($tests);
    foreach($tests as $test) {
      $class = (string)$test;
      Console::output(sprintf('- Running %s', $class));
      $obj = $this->app->decoupler()->injectInstance($class);
      if(!$obj) {
        throw new Exception(sprintf("Unable to instantiate %s", $class));
      }
      try {
        $res = await $this->app->decoupler()->awaitInjectMethod($obj, 'execute');
        Console::output('[PASSED]');
        $passed++;
        try {
          $assertions += (int)$this->app->decoupler()->injectMethod($obj, 'assertions');
        } catch(Exception $e) {}
      } catch(Exception $e) {
        Console::output('[FAILED] ' . $e->getMessage());
        exit();
      }
    }
    Console::output('All passed. ' . $passed . ' tests and ' . $assertions . ' assertions passed.');
  }
}
