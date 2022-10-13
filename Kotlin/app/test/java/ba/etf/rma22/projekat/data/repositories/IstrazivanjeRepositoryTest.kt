package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.data.models.Istrazivanje
import ba.etf.rma22.projekat.data.staticdata.istrazivanja
import org.hamcrest.Matchers.*
import org.junit.Assert.*
import org.hamcrest.CoreMatchers.`is` as Is

import org.junit.Test

class IstrazivanjeRepositoryTest {

    /*@Test
    fun getIstrazivanjeByGodina() {
        val istrazivanja = IstrazivanjeRepository.getIstrazivanjeByGodina(1)
        assertEquals(istrazivanja.size,2)
        assertEquals(istrazivanja.get(0).naziv,"Istraživanje 1/1")
        assertEquals(istrazivanja.get(1).naziv,"Istraživanje 2/1")
    }

    @Test
    fun getAll() {
        val istrazivanja = IstrazivanjeRepository.getAll()
        assertEquals(istrazivanja.size,8)
        assertThat(istrazivanja,hasItem<Istrazivanje>(hasProperty("naziv",Is("Istraživanje 1/5"))))
        assertThat(istrazivanja,not(hasItem<Istrazivanje>(hasProperty("naziv",Is("Istraživanje 2/4")))))
    }

    @Test
    fun getUpisani() {
        val istrazivanja = IstrazivanjeRepository.getUpisani()
        assertThat(istrazivanja,hasItem<Istrazivanje>(hasProperty("naziv",Is("Istraživanje 2/3"))))
        assertThat(istrazivanja,not(hasItem<Istrazivanje>(hasProperty("naziv",Is("Istraživanje 2/1")))))
    }

    @Test
    fun upisiIstrazivanje() {
        var istrazivanja = IstrazivanjeRepository.getUpisani()
        val istrazivanje = Istrazivanje(0,"Istraživanje 1/2",2)
        assert(!istrazivanja.contains(istrazivanje))
        IstrazivanjeRepository.upisiIstrazivanje(istrazivanje.naziv,"Grupa 1")
        istrazivanja=IstrazivanjeRepository.getUpisani()
        assertEquals(istrazivanja.last().naziv,istrazivanje.naziv)
        assertEquals(istrazivanja.last().godina,istrazivanje.godina)
    }

    @Test
    fun getUpisanaIstrazivanja() {
        val istrazivanja = IstrazivanjeRepository.getUpisanaIstrazivanja()
        assertThat(istrazivanja.keys.toList(),hasItem<Istrazivanje>(hasProperty("naziv",Is("Istraživanje 2/2"))))
        assertThat(istrazivanja.keys.toList(),not(hasItem<Istrazivanje>(hasProperty("naziv",Is("Istraživanje 2/1")))))
    }*/
}