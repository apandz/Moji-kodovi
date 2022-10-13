package ba.etf.rma22.projekat.data.repositories

import ba.etf.rma22.projekat.data.models.Anketa
import org.hamcrest.Matchers.*
import org.junit.Assert.assertEquals
import org.junit.Assert.assertThat
import org.junit.Test
import org.hamcrest.CoreMatchers.`is` as Is


class AnketaRepositoryTest {

  /*  @Test
    fun getMyAnkete() {
        val ankete = AnketaRepository.getMyAnkete()
        assertEquals(ankete.size,3)
        assertEquals(ankete.get(0).naziv, "Anketa 1")
        assertEquals(ankete.get(0).nazivIstrazivanja,"Istraživanje 1/1")
        assertEquals(ankete.get(0).nazivGrupe,"Grupa 1")
        assertThat(ankete,hasItem<Anketa>(hasProperty("nazivIstrazivanja",Is("Istraživanje 2/2"))))
        assertThat(ankete,not(hasItem<Anketa>(hasProperty("nazivIstrazivanja",Is("Istraživanje 1/4")))))
    }

    @Test
    fun getAll() {
        val ankete = AnketaRepository.getAll()
        assertEquals(ankete.size,19)
        assertEquals(ankete.last().naziv, "Anketa 2")
        assertEquals(ankete.last().nazivIstrazivanja,"Istraživanje 1/5")
        assertEquals(ankete.last().nazivGrupe,"Grupa 2")
        assertThat(ankete,hasItem<Anketa>(hasProperty("nazivIstrazivanja",Is("Istraživanje 1/4"))))
        assertThat(ankete,not(hasItem<Anketa>(hasProperty("nazivIstrazivanja",Is("Istraživanje 2/4")))))
    }

    @Test
    fun getDone() {
        val ankete = AnketaRepository.getDone()
        assertEquals(ankete.size,1)
        assertEquals(ankete.get(0).naziv, "Anketa 1")
        assertEquals(ankete.get(0).nazivIstrazivanja,"Istraživanje 1/1")
        assertEquals(ankete.get(0).nazivGrupe,"Grupa 1")
    }

    @Test
    fun getFuture() {
        val ankete = AnketaRepository.getFuture()
        assertEquals(ankete.size,2)
        assertEquals(ankete.get(0).naziv, "Anketa 2")
        assertEquals(ankete.get(0).nazivIstrazivanja,"Istraživanje 2/2")
        assertEquals(ankete.get(0).nazivGrupe,"Grupa 2")
        assertThat(ankete,hasItem<Anketa>(hasProperty("nazivIstrazivanja",Is("Istraživanje 2/3"))))
        assertThat(ankete,not(hasItem<Anketa>(hasProperty("nazivIstrazivanja",Is("Istraživanje 1/2")))))
    }

    @Test
    fun getNotTaken() {
        val ankete = AnketaRepository.getNotTaken()
        assertEquals(ankete.size,0)
    }*/
}