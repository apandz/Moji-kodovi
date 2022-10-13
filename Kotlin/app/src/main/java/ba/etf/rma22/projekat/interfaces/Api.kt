package ba.etf.rma22.projekat.interfaces

import ba.etf.rma22.projekat.data.models.*
import retrofit2.Response
import retrofit2.http.*

interface Api {

    //Istrazivanje
    @GET("/istrazivanje")
    suspend fun getIstrazivanja(
        @Query("offset") offset : Int
    ): Response<List<Istrazivanje>>

    @GET("/istrazivanje/{id}")
    suspend fun getIstrazivanje(
        @Path("id") istrazivanje : Int
    ): Response<Istrazivanje>

    @GET("/grupa/{gid}/istrazivanje")
    suspend fun getIstrazivanjaZaGrupu(
        @Path("gid") grupa : Int
    ): Response<List<Istrazivanje>>


    //Grupa
    @GET("/anketa/{id}/grupa")
    suspend fun getGrupeZaAnketu(
        @Path("gid") gid : Int
    ): Response<List<Grupa>>

    @POST("/grupa/{gid}/student/{id}")
    suspend fun upisiUGrupu(
        @Path("gid") grupa : Int, @Path("id") student : String
    ): Response<Poruka>

    @GET("/student/{id}/grupa")
    suspend fun getUpisaneGrupe(
        @Path("id") student : String
    ): Response<List<Grupa>>

    @GET("/grupa")
    suspend fun getGrupe(): Response<List<Grupa>>

    @GET("/grupa/{id}")
    suspend fun getGrupa(
        @Path("id") grupa : Int
    ): Response<Grupa>


    //Anketa
    @GET("/anketa")
    suspend fun getAll(
        @Query("offset") offset : Int
    ): Response<List<Anketa>>

    @GET("/anketa/{id}")
    suspend fun getById(
        @Path("id") anketa : Int
    ): Response<Anketa>

    @GET("/grupa/{id}/ankete")
    suspend fun getUpisane(
        @Path("id") grupa : Int
    ): Response<List<Anketa>>


    //Odgovor
    @GET("/student/{id}/anketataken/{ktid}/odgovori")
    suspend fun getOdgovoriAnketa(
        @Path("id") student : String, @Path("ktid") anketataken : Int
    ): Response<List<Odgovor>>

    @POST("/student/{id}/anketataken/{ktid}/odgovor")
    suspend fun postaviOdgovorAnketa(
        @Path("id") student : String, @Path("ktid") anketataken : Int, @Body podaci : Podaci
    ): Response<Odgovor>


    //AnketaTaken
    @GET("/student/{id}/anketataken")
    suspend fun getPoceteAnkete(
        @Path("id") student : String
    ): Response<List<AnketaTaken>>

    @POST("/student/{id}/anketa/{kid}")
    suspend fun zapocniAnketu(
        @Path("id") student : String, @Path("kid") anketa : Int
    ): Response<AnketaTaken>


    //Account
    @GET("/student/{id}")
    suspend fun getAccount(
        @Path("id") student : String
    ): Response<Account>

    @DELETE("/student/{id}/upisugrupeipokusaji")
    suspend fun deleteSve(
        @Path("id") student : String
    ): Response<Poruka>


    //Pitanje
    @GET("/anketa/{id}/pitanja")
    suspend fun getPitanja(
        @Path("id") anketa : Int
    ): Response<List<Pitanje>>
}