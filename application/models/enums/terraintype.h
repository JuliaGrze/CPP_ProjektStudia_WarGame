#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H

/**
 * @brief Typ terenu na planszy gry.
 *
 * Wyliczenie określa rodzaje pól, które wpływają na rozgrywkę,
 * m.in. na koszt ruchu, obronę oraz możliwości przemieszczania się jednostek.
 */
enum class TerrainType
{
    Plain,     /**< Równina – brak bonusów i utrudnień. */
    Forest,    /**< Las – zapewnia osłonę i utrudnia trafienie. */
    Mountain,  /**< Wzgórza/góry – zwiększona osłona, możliwy bonus zasięgu. */
    Water,     /**< Woda – zazwyczaj nieprzechodnia dla jednostek lądowych. */
    Building,  /**< Budynek – duża osłona dla jednostek. */
    Bridge     /**< Most – umożliwia przejście przez wodę. */
};

#endif // TERRAINTYPE_H
