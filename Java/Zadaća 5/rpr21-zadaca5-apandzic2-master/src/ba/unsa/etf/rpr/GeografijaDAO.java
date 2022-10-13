package ba.unsa.etf.rpr;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.sql.*;
import java.util.ArrayList;
import java.util.Scanner;

public class GeografijaDAO {
    private static GeografijaDAO instance =null;
    private Connection conn;
    private PreparedStatement gradSvi, drzavaSvi, noviIdGrad, noviIdDrzava, glavniGradUpit, nadjiGradUpit,nadjiDrzavuUpit, izbrisiGradoveUpit, izbrisiDrzavuUpit, dodajGradUpit, dodajDrzavuUpit, azurirajGradUpit, obrisiGradUpit;

    public Connection getConn() {
        return conn;
    }

    private GeografijaDAO() {
        String url = "jdbc:sqlite:baza.db";
        try {
            conn = DriverManager.getConnection(url);
            try {
                gradSvi = conn.prepareStatement("SELECT * FROM grad ORDER BY broj_stanovnika DESC");
                drzavaSvi = conn.prepareStatement("SELECT * FROM drzava");
                noviIdGrad = conn.prepareStatement("SELECT MAX(id)+1 FROM grad");
                noviIdDrzava = conn.prepareStatement("SELECT MAX(id)+1 FROM drzava");
                glavniGradUpit = conn.prepareStatement("SELECT g.id, g.naziv, g.broj_stanovnika, g.drzava, d.naziv, g.slika, g.postanski_broj FROM grad g, drzava d WHERE g.id=d.glavni_grad AND d.naziv=?");
                nadjiGradUpit = conn.prepareStatement("SELECT g.id, g.naziv, g.broj_stanovnika, g.drzava, d.naziv, g.slika, g.postanski_broj FROM grad g, drzava d WHERE d.id=g.drzava AND g.naziv=?");
                nadjiDrzavuUpit = conn.prepareStatement("SELECT id FROM drzava WHERE naziv=?");
                izbrisiGradoveUpit = conn.prepareStatement("DELETE FROM grad WHERE drzava=?");
                izbrisiDrzavuUpit = conn.prepareStatement("DELETE FROM drzava WHERE id=?");
                dodajGradUpit = conn.prepareStatement("INSERT INTO grad VALUES (?,?,?,?,?,?)");
                dodajDrzavuUpit = conn.prepareStatement("INSERT INTO drzava VALUES (?,?,?)");
                azurirajGradUpit = conn.prepareStatement("UPDATE grad SET naziv=?, broj_stanovnika=?, drzava=?, slika=?, postanski_broj=? WHERE id=?");
                obrisiGradUpit=conn.prepareStatement("DELETE FROM grad WHERE id=?");
            } catch (SQLException e) {
                kreirajBazu();
                gradSvi = conn.prepareStatement("SELECT * FROM grad ORDER BY broj_stanovnika DESC");
                drzavaSvi = conn.prepareStatement("SELECT * FROM drzava");
                noviIdGrad = conn.prepareStatement("SELECT MAX(id)+1 FROM grad");
                noviIdDrzava = conn.prepareStatement("SELECT MAX(id)+1 FROM drzava");
                glavniGradUpit = conn.prepareStatement("SELECT g.id, g.naziv, g.broj_stanovnika, g.drzava, d.naziv, g.slika, g.postanski_broj FROM grad g, drzava d WHERE g.id=d.glavni_grad AND d.naziv=?");
                nadjiGradUpit = conn.prepareStatement("SELECT g.id, g.naziv, g.broj_stanovnika, g.drzava, d.naziv, g.slika, g.postanski_broj FROM grad g, drzava d WHERE d.id=g.drzava AND g.naziv=?");
                nadjiDrzavuUpit = conn.prepareStatement("SELECT id FROM drzava WHERE naziv=?");
                izbrisiGradoveUpit = conn.prepareStatement("DELETE FROM grad WHERE drzava=?");
                izbrisiDrzavuUpit = conn.prepareStatement("DELETE FROM drzava WHERE id=?");
                dodajGradUpit = conn.prepareStatement("INSERT INTO grad VALUES (?,?,?,?,?,?)");
                dodajDrzavuUpit = conn.prepareStatement("INSERT INTO drzava VALUES (?,?,?)");
                azurirajGradUpit = conn.prepareStatement("UPDATE grad SET naziv=?, broj_stanovnika=?, drzava=?, slika=?, postanski_broj=? WHERE id=?");
                obrisiGradUpit=conn.prepareStatement("DELETE FROM grad WHERE id=?");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }


    private void kreirajBazu() {
        Scanner ulaz = null;
        try {
            ulaz = new Scanner(new FileInputStream("baza.sql"));
            StringBuilder sqlUpit = new StringBuilder();
            while (ulaz.hasNext()) {
                sqlUpit.append(ulaz.nextLine());
                if ( sqlUpit.length() > 1 && sqlUpit.charAt( sqlUpit.length()-1 ) == ';') {
                    try {
                        Statement stmt = conn.createStatement();
                        stmt.execute(sqlUpit.toString());
                        sqlUpit = new StringBuilder();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
            }
            ulaz.close();
        } catch (FileNotFoundException e) {
            System.out.println("Ne postoji SQL datoteka... nastavljam sa praznom bazom");
        }
    }

    public static GeografijaDAO getInstance() {
        if(instance==null) instance=new GeografijaDAO();
        return instance;
    }

    public static void removeInstance() {
        if(instance!=null) {
            try {
                instance.conn.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        instance=null;
    }

    public ArrayList<Grad> gradovi() {
        ArrayList<Grad> gradovi=new ArrayList<>();
        ArrayList<Drzava> drzave=new ArrayList<>();
        try {
            ResultSet rs=gradSvi.executeQuery();
            ResultSet drs=drzavaSvi.executeQuery();
            while(drs.next()) {
                drzave.add(new Drzava(drs.getInt(1),drs.getString(2),new Grad(drs.getInt(3),"",0,null)));
            }
            while(rs.next()) {
                Drzava d=new Drzava(rs.getInt(4),"",null);
                for(Drzava dd: drzave) {
                    if(rs.getInt(4)==dd.getId()) {
                        d=dd;
                        break;
                    }
                }
                Grad grad=new Grad(rs.getInt(1),rs.getString(2),rs.getInt(3),d);
                grad.setSlika(rs.getString(5));
                grad.setPostanskiBroj(rs.getInt(6));
                gradovi.add(grad);
                if(d.getGlavniGrad()!=null && rs.getInt(1)==d.getGlavniGrad().getId()) {
                    d.setGlavniGrad(gradovi.get(gradovi.size()-1));
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return gradovi;
    }

    private int dajNoviIdGrad() {
        try {
            ResultSet rs=noviIdGrad.executeQuery();
            if(rs.next()) return rs.getInt(1);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return 1;
    }

    private int dajNoviIdDrzava() {
        try {
            ResultSet rs=noviIdDrzava.executeQuery();
            if(rs.next()) return rs.getInt(1);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return 1;
    }

    public Grad glavniGrad(String drzava) {
        Grad g=null;
        try {
            glavniGradUpit.setString(1,drzava);
            ResultSet rs=glavniGradUpit.executeQuery();
            if(rs.next()) {
                g=new Grad(rs.getInt(1),rs.getString(2),rs.getInt(3),new Drzava(rs.getInt(4),rs.getString(5),g));
                String slika=rs.getString(6);
                if(slika!=null){
                    g.setSlika(slika);
                } else {
                    g.setSlika("");
                }
                g.setPostanskiBroj(rs.getInt(7));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return g;
    }

    public void obrisiDrzavu(String drzava) {
        try {
            nadjiDrzavuUpit.setString(1,drzava);
            ResultSet rs=nadjiDrzavuUpit.executeQuery();
            if(rs.next()) {
                izbrisiGradoveUpit.setInt(1,rs.getInt(1));
                izbrisiGradoveUpit.executeUpdate();
                izbrisiDrzavuUpit.setInt(1,rs.getInt(1));
                izbrisiDrzavuUpit.executeUpdate();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void dodajGrad(Grad grad) {
        try {
            if(grad.getId()==0) {
                int id=dajNoviIdGrad();
                dodajGradUpit.setInt(1, id);
                grad.setId(id);
            } else {
                dodajGradUpit.setInt(1,grad.getId());
            }
            dodajGradUpit.setString(2,grad.getNaziv());
            dodajGradUpit.setInt(3,grad.getBrojStanovnika());
            if(grad.getDrzava()!=null) {
                if(grad.getDrzava().getId()==0) {
                    int id=dajNoviIdGrad();
                    grad.getDrzava().setId(id);
                }
                dodajGradUpit.setInt(4,grad.getDrzava().getId());
            }
            dodajGradUpit.setString(5,grad.getSlika());
            dodajGradUpit.setInt(6,grad.getPostanskiBroj());
            dodajGradUpit.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void dodajDrzavu(Drzava drzava) {
        try {
            if(drzava.getId()==0) {
                int id=dajNoviIdDrzava();
                dodajDrzavuUpit.setInt(1, id);
                drzava.setId(id);
            } else {
                dodajDrzavuUpit.setInt(1,drzava.getId());
            }
            dodajDrzavuUpit.setString(2,drzava.getNaziv());
            if(drzava.getGlavniGrad()!=null) {
                if(drzava.getGlavniGrad().getId()==0) {
                    int id=dajNoviIdGrad();
                    drzava.getGlavniGrad().setId(id);
                }
                dodajDrzavuUpit.setInt(3,drzava.getGlavniGrad().getId());
            }
            dodajDrzavuUpit.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void izmijeniGrad(Grad grad) {
        try {
            azurirajGradUpit.setString(1,grad.getNaziv());
            azurirajGradUpit.setInt(2,grad.getBrojStanovnika());
            azurirajGradUpit.setInt(3, grad.getDrzava().getId());
            azurirajGradUpit.setString(4,grad.getSlika());
            azurirajGradUpit.setInt(5,grad.getPostanskiBroj());
            azurirajGradUpit.setInt(6,grad.getId());
            azurirajGradUpit.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public Drzava nadjiDrzavu(String drzava) {
        Drzava d=null;
        try {
            glavniGradUpit.setString(1,drzava);
            ResultSet rs=glavniGradUpit.executeQuery();
            if(rs.next()) {
                Grad grad=new Grad(rs.getInt(1),rs.getString(2),rs.getInt(3),d);
                grad.setSlika(rs.getString(6));
                grad.setPostanskiBroj(rs.getInt(7));
                d=new Drzava(rs.getInt(4),rs.getString(5),grad);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return d;
    }

    public void vratiBazuNaDefault() {
        Statement stmt = null;
        try {
            stmt = conn.createStatement();
            stmt.executeUpdate("DELETE FROM grad");
            stmt.executeUpdate("DELETE FROM drzava");
            kreirajBazu();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public ArrayList<Drzava> drzave() {
        ArrayList<Grad> gradovi=new ArrayList<>();
        ArrayList<Drzava> drzave=new ArrayList<>();
        try {
            ResultSet rs=gradSvi.executeQuery();
            ResultSet drs=drzavaSvi.executeQuery();
            while(drs.next()) {
                drzave.add(new Drzava(drs.getInt(1),drs.getString(2),new Grad(drs.getInt(3),"",0,null)));
            }
            while(rs.next()) {
                Drzava d=new Drzava(rs.getInt(4),"",null);
                for(Drzava dd: drzave) {
                    if(rs.getInt(4)==dd.getId()) {
                        d=dd;
                        break;
                    }
                }
                Grad grad=new Grad(rs.getInt(1),rs.getString(2),rs.getInt(3),d);
                grad.setSlika(rs.getString(5));
                grad.setPostanskiBroj(rs.getInt(6));
                gradovi.add(grad);
                if(d.getGlavniGrad()!=null && rs.getInt(1)==d.getGlavniGrad().getId()) {
                    d.setGlavniGrad(gradovi.get(gradovi.size()-1));
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return drzave;
    }

    public void obrisiGrad(int id) {
        try {
            obrisiGradUpit.setInt(1,id);
            obrisiGradUpit.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public Grad nadjiGrad(String naziv) {
        Grad g=null;
        try {
            nadjiGradUpit.setString(1,naziv);
            ResultSet rs=nadjiGradUpit.executeQuery();
            if(rs.next()) {
                g=new Grad(rs.getInt(1),rs.getString(2),rs.getInt(3),new Drzava(rs.getInt(4),rs.getString(5),g));
                String slika=rs.getString(6);
                if(slika!=null){
                    g.setSlika(slika);
                } else {
                    g.setSlika("");
                }
                g.setPostanskiBroj(rs.getInt(7));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return g;
    }
}

