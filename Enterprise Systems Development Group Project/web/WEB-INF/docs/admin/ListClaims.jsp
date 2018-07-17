<%-- 
    Document   : ClaimsLIst
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ include file="/resources/AdminDashboardNavBar.jsp" %>

<div class="content">
    <h1>Claims</h1>

    <div style="display: none;">
        ${JDBCBean.executeSQLQuery("SELECT * FROM ROOT.CLAIMS WHERE \"status\"='APPROVED'")}
    </div>

    <h2>Approved Claims</h2>
    <table>
        <tr>
            <th>ID</th>
            <th>Member ID</th>
            <th>Date</th>
            <th>Rationale</th>
            <th>Status</th>
            <th>Amount</th>
        </tr>
        <c:forEach items="${JDBCBean.resultsToArrayList()}" var="row" varStatus="rowStatus">
            <tr>
                <c:forEach items="${row}" var="column" varStatus="columnStatus">
                    <c:choose>
                        <c:when test="${columnStatus.last}">
                            <td>£${column}</td>                           
                        </c:when>
                        <c:otherwise>
                            <td>${column}</td>
                        </c:otherwise>
                    </c:choose>
                </c:forEach>
            </tr>
        </c:forEach>
    </table>
    <br><br>

    <div style="display: none;">
        ${JDBCBean.executeSQLQuery("SELECT * FROM ROOT.CLAIMS WHERE \"status\"='SUBMITTED'")}
    </div>

    <h2>Submitted Claims</h2>
    <form action="${pageContext.request.contextPath}/AdminController" method="post">
        <table>
            <tr>
                <th>ID</th>
                <th>Member ID</th>
                <th>Date</th>
                <th>Rationale</th>
                <th>Status</th>
                <th>Amount</th>
                <th>Select</th> 
            </tr>
            <c:forEach items="${JDBCBean.resultsToArrayList()}" var="row" varStatus="rowStatus">
                <tr>
                    <c:forEach items="${row}" var="column" varStatus="columnStatus">
                        <c:choose>
                            <c:when test="${columnStatus.last}">
                                <td>£${column}</td>
                                <td><input class="radio" type="radio" name="selectedClaim" value="${row[0]}" ${rowStatus.first ? 'checked="checked"' : ''}/></td>
                                </c:when>
                                <c:otherwise>
                                <td>${column}</td>
                            </c:otherwise>
                        </c:choose>
                    </c:forEach>
                </tr>
            </c:forEach>
        </table>
        <br><br>
        <input type="hidden" name="viewId" value="/ListClaims">
        <input class="btn" type="submit" value="View Selected"/>
    </form>
</div>