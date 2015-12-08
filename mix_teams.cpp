#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <pqxx/pqxx>

using namespace std;

int main(int argc, char const *argv[]) {
    cout << "Blåbærsyltetøj!" << endl;
    std::string league = "";
    std::string season = "";
    if (argc != 3) {
        cout << "usage: " << argv[0] << " [league] [season]" << endl;
        exit(1);
    } else {
        league = argv[1];
        season = argv[2];
    }
    std::vector<string> teams_vector;
    std::vector<std::pair<std::string, std::string>> teams_pair;

    try {
        string sql = "";
        pqxx::connection C("dbname=sports user=claus hostaddr=127.0.0.1 port=5432");
        if (C.is_open()) {
            std::cout << "Connected to database" << std::endl;
        } else {
            std::cout << "Unable to connect to database" << std::endl;
        }
        sql = "select * from teams where league = '" + league + "' and season = '" + season + "'";
        cout << "query: " << sql << endl;
        pqxx::nontransaction N(C);
        pqxx::result R(N.exec(sql));
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            cout << "league: " << c[1].as<std::string>() << ", season: " << c[2].as<std::string>() << ", team: " << c[3].as<std::string>() << endl;
            teams_vector.emplace_back(c[3].as<std::string>());
        }
        C.disconnect();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }


    cout << "size: " << teams_vector.size() << endl;
    auto c = 0, d = 0;
    for (auto i: teams_vector) {
        for (auto m: teams_vector) {
            if (i != m) {
                ++c;
                teams_pair.emplace_back(make_pair(i, m));
            }
        }
    }
    cout << "iterations: " << c << endl;
    for (auto p: teams_pair) {
        std::string query = "";
        ++d;
        query = "insert into matches (league, season, hometeam, awayteam) values ('" + league + "', '" + season + "', '";
        query += std::get<0>(p) + "', '" + std::get<1>(p) + "');";
        cout << query << endl;

        try {
            pqxx::connection C("dbname=sports user=claus hostaddr=127.0.0.1 port=5432");
            if (C.is_open()) {
                std::cout << "Connected to database" << std::endl;
            } else {
                std::cout << "Unable to connect to database" << std::endl;
            }
            
            pqxx::work W(C);

            W.exec(query);
            W.commit();
            C.disconnect();
        }
        catch(pqxx::unique_violation & e) {
            cout << "Ignore duplicate entries" << endl;
            // ignore duplicate key violation
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    cout << "iterations: " << d << endl;

    return 0;
}
