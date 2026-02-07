#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>


/**
 * Захист структур даних, що рідко оновлюються
 *
 * Уявимо що у нас багато потоків і є ресурс, який рідко змінюється, тобто у нас має відбуватися
 * читання (що безпечно для паралельного читання кількома потоками) і рідко запис, який вимагає
 * блокування, щоб уникнути race condition.
 *
 * Розглянемо таблицю, яка використовується для зберігання кешу записів DNS для перетворення доменних імен у відповідні
 * IP-адреси. Зазвичай певний запис DNS залишається незмінним протягом тривалого періоду часу — у багатьох випадках
 * записи DNS залишаються незмінними протягом років.
 *
 * Використання std::mutex для захисту структури даних є надто песимістичним, оскільки це усуне можливу
 * одночасність читання структури даних, коли вона не піддається модифікації; необхідний інший тип мьютекса.
 * Цей новий тип мьютекса зазвичай називають мьютексом читання-запису, оскільки він дозволяє два різних типи
 * використання: ексклюзивний доступ одного потоку «запису» або спільний і одночасний доступ декількох потоків «читання»
 *
 * Стандартна бібліотека C++17 надає два таких мьютекси, std:: shared_mutex і std::shared_timed_mutex.
 *
 *
 * У коді нижче функція find_entry() використовує екземпляр std::shared_lock<> для захисту спільного доступу тільки для читання ;
 * таким чином, кілька потоків можуть одночасно викликати find_entry() без проблем. З іншого боку, функція update_or_add_entry() використовує
 * екземпляр std::lock_guard<> для забезпечення ексклюзивного доступу під час оновлення таблиці;
 * Не тільки інші потоки не можуть виконувати оновлення під час виклику update_or_add_entry(), але й потоки,
 * що викликають find_entry(), також блокуються
 */

class dns_entry {};
class Dns_cache {
    std::map<std::string,dns_entry> entries;
    mutable std::shared_mutex entry_mutex;
public:
    dns_entry find_entry(std::string const& domain) const {
        std::shared_lock<std::shared_mutex> lk(entry_mutex);
        std::map<std::string,dns_entry>::const_iterator const it= entries.find(domain);
        return (it==entries.end()) ? dns_entry() : it->second;
    }
    void update_or_add_entry(std::string const& domain,
    dns_entry const& dns_details) {
        std::lock_guard<std::shared_mutex> lk(entry_mutex);
        entries[domain]=dns_details;
    }
};


int main() {



    return 0;
}
