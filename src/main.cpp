#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <memory>
#include <optional>
#include <concepts>

template<typename T>
concept collection = requires( T coll )
{
    coll.front();
    coll.empty();
    typename T::value_type;
};

template<collection Collection_t>
auto GetFront( const Collection_t& collection ) -> std::optional<typename Collection_t::value_type>
{
    return ( collection.empty() ? std::nullopt : std::optional( collection.front() ) );
}

class SpaceshipParts
{
public:
    void ReadFromFile( const std::string& filename )
    {
        std::ifstream file( filename );

        if ( file.is_open() )
        {
            std::string line;
            while ( std::getline( file, line ) )
            {
                if ( line.rfind( "engine" ) != std::string::npos )
                    m_engineParts.push_back( line );
                else if ( line.rfind( "fuselage" ) != std::string::npos )
                    m_fuselageParts.push_back( line );
                else if ( line.rfind( "cabin" ) != std::string::npos )
                    m_cabinParts.push_back( line );
                else if ( line.rfind( "wings" ) != std::string::npos )
                    m_wingsParts.push_back( line );
                else if ( line.rfind( "armor" ) != std::string::npos )
                    m_armourParts.push_back( line );
                else if ( line.rfind( "weapon" ) != std::string::npos )
                    m_weaponParts.push_back( line );
            }

            std::cout << "parts loaded from: " << filename << '\n';
        }
        else
        {
            std::cout << "failed to load parts from: " << filename << '\n';
        }
    }

    void Shuffle()
    {
        std::shuffle( m_engineParts.begin(), m_engineParts.end(), m_generator );
        std::shuffle( m_fuselageParts.begin(), m_fuselageParts.end(), m_generator );
        std::shuffle( m_cabinParts.begin(), m_cabinParts.end(), m_generator );
        std::shuffle( m_wingsParts.begin(), m_wingsParts.end(), m_generator );
        std::shuffle( m_armourParts.begin(), m_armourParts.end(), m_generator );
        std::shuffle( m_weaponParts.begin(), m_weaponParts.end(), m_generator );
    }

    std::string GetEngine() const
    {
        return GetFront( m_engineParts ).value_or( "N/A" );
    }

    std::string GetFuselage() const
    {
        return GetFront( m_fuselageParts ).value_or( "N/A" );
    }

    std::string GetCabin() const
    {
        return GetFront( m_cabinParts ).value_or( "N/A" );
    }

    std::optional<std::string> GetWing() const
    {
        std::uniform_int_distribution<std::size_t> dist{ 0, m_wingsParts.size() };
        const auto selection = dist( m_randomDevice );

        if ( selection == m_wingsParts.size() )
        {
            return std::nullopt;
        }
        else
        {
            return m_wingsParts.at( selection );
        }
    }

    std::string GetArmour() const
    {
        return GetFront( m_armourParts ).value_or( "N/A" );
    }

    std::vector<std::string> GetWeapons() const
    {
        const auto numWeapons =
            std::min( m_weaponDistribution( m_randomDevice ), m_weaponParts.size() );
        const auto beginWeapon = m_weaponParts.begin();

        return { beginWeapon, beginWeapon + numWeapons };
    }

private:
    mutable std::random_device m_randomDevice;
    std::mt19937 m_generator{ m_randomDevice() };
    std::uniform_int_distribution<std::size_t> m_weaponDistribution{ 0, 4 };
    std::vector<std::string> m_engineParts;
    std::vector<std::string> m_fuselageParts;
    std::vector<std::string> m_cabinParts;
    std::vector<std::string> m_wingsParts;
    std::vector<std::string> m_armourParts;
    std::vector<std::string> m_weaponParts;
};

class Spaceship
{
public:
    void Generate( const SpaceshipParts& parts )
    {
        m_engine = parts.GetEngine();
        m_fuselage = parts.GetFuselage();
        m_cabin = parts.GetCabin();
        m_armour = parts.GetArmour();
        m_large_wings = parts.GetWing();
        m_small_wings = parts.GetWing();
        m_weapons = parts.GetWeapons();
    }

    friend std::ostream& operator<<( std::ostream& os, const Spaceship& sp );

private:
    std::string m_engine;
    std::string m_fuselage;
    std::string m_cabin;
    std::optional<std::string> m_large_wings;
    std::optional<std::string> m_small_wings;
    std::string m_armour;
    std::vector<std::string> m_weapons;
};

std::ostream& operator<<( std::ostream& os, const Spaceship& sp )
{
    os << "\n+++ Generated spaceship +++\n  Engine: " << sp.m_engine
       << "\n  Fuselage: " << sp.m_fuselage << "\n  Cabin: " << sp.m_cabin
       << "\n  Large wings: " << sp.m_large_wings.value_or( "N/A" )
       << "\n  Small wings: " << sp.m_small_wings.value_or( "N/A" ) << "\n  Armour: " << sp.m_armour
       << "\n  Weapons (" << sp.m_weapons.size() << "): ";

    if ( !sp.m_weapons.empty() )
    {
        os << '\n';

        for ( const auto& weapon : sp.m_weapons )
        {
            os << "    " << weapon << '\n';
        }
    }
    else
    {
        os << "N/A\n";
    }

    return os;
}

int main()
{
    SpaceshipParts parts;
    parts.ReadFromFile( "vehicle_parts_additional.txt" );
    Spaceship sp;

    for ( auto i = 0; i < 4; i++ )
    {
        parts.Shuffle();

        sp.Generate( parts );
        std::cout << sp;
    }
}
