#include <bitcoin/bitcoin.hpp>
#include <obelisk/client/interface.hpp>

using namespace bc;

bool stopped = false;

void history_fetched(const std::error_code& ec,
    const blockchain::history_list& history)
{
    if (ec)
    {
        std::cerr << "balance: Failed to fetch history: "
            << ec.message() << std::endl;
        return;
    }
    uint64_t total_recv = 0, balance = 0;
    for (const auto& row: history)
    {
        uint64_t value = row.value;
        BITCOIN_ASSERT(value >= 0);
        total_recv += value;
        if (row.spend.hash == null_hash)
            balance += value;
    }
    std::cout << balance << std::endl;
    stopped = true;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: balance ADDRESS" << std::endl;
        return -1;
    }
    std::string addr = argv[1];
    payment_address payaddr;
    if (!payaddr.set_encoded(addr))
    {
        std::cerr << "balance: Invalid address." << std::endl;
        return -1;
    }
    fullnode_interface fullnode("tcp://localhost:5555");
    fullnode.blockchain.fetch_history(payaddr, history_fetched);
    while (!stopped)
    {
        fullnode.update();
        sleep(0.1);
    }
    return 0;
}
