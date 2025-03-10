/**
 * @file
 * @copyright This code is licensed under the 3-clause BSD license.\n
 *            Copyright ETH Zurich, Laboratory of Physical Chemistry, Reiher Group.\n
 *            See LICENSE.txt for details.
 */
#ifndef READUCT_TASK_H_
#define READUCT_TASK_H_

#include <Core/Interfaces/Calculator.h>
#include <Core/Log.h>
#include <Utils/Settings.h>
#include <yaml-cpp/yaml.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Scine {
namespace Core {
class Calculator;
}
namespace Readuct {

/**
 * @brief The base class for all tasks in Readuct.
 */
class Task {
 public:
  using SystemsMap = std::map<std::string, std::shared_ptr<Core::Calculator>>;

  static std::shared_ptr<Core::Calculator> copyCalculator(SystemsMap& systems, const std::string& name,
                                                          const std::string& taskName) {
    const auto findIter = systems.find(name);
    if (findIter == systems.end()) {
      throw std::runtime_error("System name '" + name + "' is missing in " + taskName);
    }
    return std::shared_ptr<Core::Calculator>(findIter->second->clone().release());
  }

  /**
   * @brief Construct a new Task.
   * @param input  The input system names for the task.
   * @param output The output system names for the task.
   * @param logger The logger to/through which all text output will be handled.
   */
  Task(std::vector<std::string> input, std::vector<std::string> output, std::shared_ptr<Core::Log> logger = nullptr)
    : _input(std::move(input)), _output(std::move(output)), _logger(std::move(logger)) {
    if (!_logger) {
      _logger = std::make_shared<Core::Log>();
    }

    if (_input.empty()) {
      throw std::runtime_error("No input systems specified!");
    }
  }

  virtual ~Task() = default;

  /**
   * @brief Getter for the tasks name.
   * @return std::string The name of the task.
   */
  virtual std::string name() const = 0;
  /**
   * @brief Executes the actual task represented by this class.
   * @param systems A map for all current and new systems.
   * @param taskSettings The settings for this run of the task.
   * @param test If true does not run task, but checks input.
   * @param observers Adds these additional observers to optimization algorithms.
   * @return true If successful.
   * @return false If not successful.
   */
  virtual bool
  run(SystemsMap& systems, Utils::UniversalSettings::ValueCollection taskSettings, bool test = false,
      std::vector<std::function<void(const int&, const Utils::AtomCollection&, const Utils::Results&, const std::string&)>>
          observers = {}) const = 0;
  /**
   * @brief Getter for the expected names of the input systems.
   * @return const std::vector<std::string>& The names of all systems expected as inputs.
   */
  const std::vector<std::string>& input() const {
    return _input;
  };
  /**
   * @brief Getter for the names of the output systems generated by this task.
   * @return const std::vector<std::string>& The names of all systems generated as output.
   */
  const std::vector<std::string>& output() const {
    return _output;
  };
  /**
   * @brief Warn if more than one input system was specified
   */
  void warningIfMultipleInputsGiven() const {
    if (_input.size() > 1) {
      _logger->warning
          << "  Warning: More than one input system was specified. Only taking first and ignoring all others.\n";
    }
  };
  /**
   * @brief Warn if more than one output system was specified
   */
  void warningIfMultipleOutputsGiven() const {
    if (_output.size() > 1) {
      _logger->warning
          << "  Warning: More than one output system was specified. Only taking first and ignoring all others.\n";
    }
  };

  bool stopOnErrorExtraction(Utils::UniversalSettings::ValueCollection& taskSettings) const {
    bool stopOnError = true;
    if (taskSettings.valueExists("allow_unconverged")) {
      _logger->warning << "  The option 'allow_unconverged' is deprecated.\n"
                       << "  It has been replaced with 'stop_on_error',\n"
                       << "  which is now available for all tasks and is defaulted to 'true'.\n\n";
      stopOnError = !taskSettings.extract("allow_unconverged", false);
    }
    return taskSettings.extract("stop_on_error", stopOnError);
  };

  static std::string falseTaskSettingsErrorMessage(const std::string& name) {
    return "  You gave Task settings for the " + name + ",\n" + "  but the only possible setting for this task, are the\n" +
           "  'stop_on_error' option to control whether ReaDuct fails\n" +
           "  with a failed calculation or simply returns false\n" +
           "  and the 'silent_stdout_calculator' option to control whether\n"
           "  the standard output of the calculator should be printed.\n"
           "  You might want to specify the settings you put into the task settings\n" +
           "  in the systems section.";
  }

 protected:
  const std::vector<std::string> _input;
  const std::vector<std::string> _output;
  std::shared_ptr<Core::Log> _logger;
};

} // namespace Readuct
} // namespace Scine

#endif // READUCT_TASK_H_
