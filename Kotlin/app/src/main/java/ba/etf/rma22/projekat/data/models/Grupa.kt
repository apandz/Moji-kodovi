package ba.etf.rma22.projekat.data.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey
import com.google.gson.annotations.SerializedName

@Entity
data class Grupa(
    @PrimaryKey @ColumnInfo(name = "id") @SerializedName("id") val id : Int,
    @ColumnInfo(name = "naziv") @SerializedName("naziv") val naziv : String,
    @ColumnInfo(name = "IstrazivanjeId") @SerializedName("IstrazivanjeId") val istrazivanjeId : Int
) {
    override fun toString(): String {
        return naziv
    }

    override fun equals(other: Any?): Boolean {
        return id==(other as Grupa).id
    }

    override fun hashCode(): Int {
        return id
    }
}
